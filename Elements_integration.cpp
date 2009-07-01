#include "Element.h"
#include "Eexpression.h"
#include "ASTNode.h"
#include "Efn.h"
#include "Eclass.h"
#include "Scope.h"
#include "Variable.h"
#include "Operator.h"
#include "Use.h"
#include "error.h"

void Eif::integration(Scope* scope) {
	condition->integration(scope);
	then->integration(scope);
	if (elsethen!=0) elsethen->integration(scope);
}

void Ewhile::integration(Scope* scope) {
	condition->integration(scope);
	body->integration(scope);
}

void Efor::integration(Scope* scope) {
	variable=registerVariable(varname);
	outer=scope;
	list->integration(scope);
	body->integration(this);
}

void Eexpression::integration(Scope* scope) {
	GYuint i, s;
	GYarray<Piece*> obody;
	GYarray<ASTNode*> nstack;
	GYarray<Operator*> ostack;
	bool infix;
	ASTNode *tar, *tar2;
	Operator *op;

	obody=body;
	nstack.clear();
	ostack.clear();
	s=body.size();
	infix=false;
	for (i=0; i<s; i++) {
		switch (body[i]->getType()) {
		case PT_ELEMENT:
			dynamic_cast<Pelement*>(body[i])->getValue()->integration(scope);
		case PT_BOOL:
		case PT_INT:
		case PT_FLOAT:
		case PT_STR:
		case PT_FN:
		case PT_CLASS:
		case PT_LIST:
		case PT_VARIABLE:
			nstack.push_back(new Nelement(body[i]));
			infix=true;
			break;
		case PT_OPERATOR:
			op=(dynamic_cast<Poperators*>(body[i]))->getValue()->findinfix(infix);
			if (op==0) error(ERROR_INTEGRATION_CANNOTFINDSYMBOL);		// 오류
			if (infix) {
				while ((!ostack.empty() && ostack.back()->getOperatorType()==PREFIX) &&
					op->getPriority()>=ostack.back()->getPriority()) {			// oeprator priority
					tar=nstack.back(); nstack.pop_back();
					nstack.push_back(new Nuncall(ostack.back(), tar));
					ostack.pop_back();
				}
				while (!ostack.empty() && 
					(ostack.back()->getOperatorType()==INFIXL || ostack.back()->getOperatorType()==INFIXR) &&
					(op->getPriority()>=ostack.back()->getPriority())) {
					tar=nstack.back(); nstack.pop_back();
					tar2=nstack.back(); nstack.pop_back();
					nstack.push_back(new Nbincall(ostack.back(), tar, tar2));
					ostack.pop_back();
				}
			}
			ostack.push_back(op);
			infix=false;
			break;
		case PT_DOT:
			if (nstack.back()->getObjType()==PT_CLASS) {
				// 결정된
				Piece *pc=(dynamic_cast<Nelement*>(nstack.back()))->getPiece();
				Eclass *clss=(dynamic_cast<Pclass*>(pc)->getValue());
				Eclass *ncl;
				GYstring name;

				i++;
				if (i>=body.size() || (dynamic_cast<Pidentifier*>(body[i]))==0) 
					error(ERROR_INTEGRATION_WRONGDOT);					// 오류
				name=(dynamic_cast<Pidentifier*>(body[i]))->getName();
				nstack.pop_back();
				ncl=clss->findClass(name);
				if (ncl!=0) body[i]=new Pclass(ncl);
				else {
					Operators *ops=clss->findOperators(name);
					Efns *fns=clss->findFn(name);

					if (ops!=0) body[i]=new Poperators(ops);
					else if (fns->getLength()>0) body[i]=new Pfns(fns);
					else error(ERROR_INTEGRATION_CANNOTFINDSYMBOL);		// 오류
				}
				i--;
			} else {
				// 결정되지 않은
				ASTNode *node;
				GYstring name;

				i++;
				if (i>=body.size() || (dynamic_cast<Pidentifier*>(body[i]))==0)
					error(ERROR_INTEGRATION_WRONGDOT);					// 오류
				name=(dynamic_cast<Pidentifier*>(body[i]))->getName();
				node=nstack.back(); nstack.pop_back();
				nstack.push_back(new Ndot(node, name));
			}
			break;
		case PT_CALL: {
			GYarray<Element*> *list;
			int j, js;

			list=dynamic_cast<Pcall*>(body[i])->getListPointer();
			js=list->size();
			for (j=0; j<js; j++) {
				Element *el;

				el=list->at(j);
				el->integration(scope);
			}
			if (!nstack.empty() && nstack.back()->getObjType()==PT_FN && infix) {
				// 함수를 호출하려는 경우
				Piece *pc=(dynamic_cast<Nelement*>(nstack.back()))->getPiece();
				Efns *fns=(dynamic_cast<Pfns*>(pc))->getValue();
				
				nstack.pop_back();
				nstack.push_back(new Nfncall(fns, dynamic_cast<Pcall*>(body[i])));
			} else if (!nstack.empty() && nstack.back()->getObjType()==PT_CLASS && infix) {
				// 클래스를 호출하려는 경우
				Piece *pc=(dynamic_cast<Nelement*>(nstack.back()))->getPiece();
				Eclass *clss=(dynamic_cast<Pclass*>(pc))->getValue();
				
				nstack.pop_back();
				nstack.push_back(new Nclasscall(clss, dynamic_cast<Pcall*>(body[i])));
			} else if (!nstack.empty() && nstack.back()->getObjType()==PT_VARIABLE && infix) {
				// 호출 대상이 변수인 경우
				Piece *pc=(dynamic_cast<Nelement*>(nstack.back()))->getPiece();
				Variable *var=(dynamic_cast<Pvariable*>(pc))->getValue();
				
				nstack.pop_back();
				nstack.push_back(new Nvariablecall(var, dynamic_cast<Pcall*>(body[i])));
			} else if (!nstack.empty() &&  (nstack.back()->getType()==NODE_FNCALL || 
											nstack.back()->getType()==NODE_CLASSCALL ||
											nstack.back()->getType()==NODE_ANONYMCALL ||
											nstack.back()->getType()==NODE_VALUECALL ||
											nstack.back()->getType()==NODE_VARIABLECALL ||
											nstack.back()->getType()==NODE_ELEMENT) && infix) {
				// 호출 대상이 값인 경우
				ASTNode *node;
				
				node=nstack.back();
				nstack.pop_back();
				nstack.push_back(new Nvaluecall(node, dynamic_cast<Pcall*>(body[i])));
			} else if (!nstack.empty() && nstack.back()->getType()==NODE_DOT && infix) {
				// 뭔가 호출하려고는 하는데 알 수 없는 경우
				Ndot *obj=dynamic_cast<Ndot*>(nstack.back());
				
				nstack.pop_back();
				nstack.push_back(new Nanonymcall(obj, dynamic_cast<Pcall*>(body[i])));
			} else {
				if (dynamic_cast<Pcall*>(body[i])->getCallType()==CT_ROUND) {
					if (list->size()==1) {
						// 괄호로 우선순위를 바꾸는 것이거나
						// (+)나 (-)처럼 연산자를 함수처럼 쓰려는 것이거나
						Eexpression *expr=dynamic_cast<Eexpression*>(list->at(0));
						ASTNode *node;

						node=expr->getASTNode();
						if (node->getType()==NODE_OPERATOR) {
							// 연산자를 함수처럼 쓰려는 경우
							body[i]=new Pfns(dynamic_cast<Noperator*>(node)->getOperator()->getFns());
							i--;
						} else {
							// 괄호로 우선순위를 바꾸려는 경우
							nstack.push_back(node);
						}
					} else ;				// 오류?
				} else {
					// 리스트로 쓰려는 경우
					Plist *plist;
					unsigned int j, s;

					plist=new Plist();
					s=list->size();
					for (j=0; j<s; j++)
						plist->getListPointer()->push_back(list->at(j));
//					delete body[i];
					body[i]=plist;
					i--;
				}
			}
			infix=true;
					  } break;
		case PT_IDENTIFIER: {
			GYstring name;
			Eclass *cls;
			Variable *var;

			name=dynamic_cast<Pidentifier*>(body[i])->getName();
//			delete body[i];
			cls=scope->findClass(name);
			var=scope->findVariable(name);
			if (i+1<s && body[i+1]->getType()==PT_DOT) {
				if (var!=0) body[i]=new Pvariable(var);
				else if (cls!=0) body[i]=new Pclass(cls);
				else body[i]=new Pvariable(scope->registerVariable(name));
			} else {
				Operators *opr;
				Efns *fns;

				opr=scope->findOperators(name);
				if (opr!=0) body[i]=new Poperators(opr);
				else if (var!=0) body[i]=new Pvariable(var);
				else {
					fns=scope->findFn(name);
					if (fns->getLength()>0) body[i]=new Pfns(fns);
					else {
//						delete fns;
						if (cls!=0) body[i]=new Pclass(cls);
						else body[i]=new Pvariable(scope->registerVariable(name));
					}
				}
			}
			i--;
							} break;
		}
	}
	if (nstack.empty() && ostack.size()==1) {
		ast=new Noperator(ostack[0]);
		return;
	}
	while (!ostack.empty()) {
		if (ostack.back()->getOperatorType()==PREFIX) {
			tar=nstack.back(); nstack.pop_back();
			nstack.push_back(new Nuncall(ostack.back(), tar));
			ostack.pop_back();
		} else {
			tar=nstack.back(); nstack.pop_back();
			tar2=nstack.back(); nstack.pop_back();
			nstack.push_back(new Nbincall(ostack.back(), tar, tar2));
			ostack.pop_back();
		}
	}
	body=obody;
	if (!nstack.empty()) ast=nstack.back();
	else ast=0;
}

void Eblock::integration(Scope* scope) {
	GYuint i, s;
	GYarray<Use*>::iterator it;
	GYmap<GYstring, Eclass*>::iterator cit;
	GYmap<GYstring, Operators*>::iterator ops;
	Operator *op;
	Efn* f;

	outer=scope;

	it=uses.begin();
	while (it!=uses.end()) {
		(*it)->integration(this);
		it++;
	}

	s=body.size();
	for (i=0; i<s; i++)
		body[i]->integration(this);

	ops=operators.begin();
	while (ops!=operators.end()) {
		for (GYuint i=0, s=ops->second->size(); i<s; i++) {
			op=ops->second->at(i);
			op->setFns(findFn(op->getName()));
		}
		ops++;
	}
	for (i=0, s=functions.size(); i<s; i++) {
		f=functions[i];
		f->integration(this);
	}
	for (cit=classes.begin(); cit!=classes.end(); cit++)
		cit->second->integration(this);
}

void Eassign::integration(Scope* scope) {
	target=scope->findVariable(targetname);
	if (target==0)
		target=scope->registerVariable(targetname);
	body->integration(scope);
}

void Ereturn::integration(Scope* scope) {
	body->integration(scope);
}

void FNparam::integration(Scope* scope) {
	GYuint i, s;
	Eclass *clss;

	s=typenames.size();
	for (i=0; i<s; i++) {
		clss=scope->findClass(typenames[i]);
		if (clss==0) error(ERROR_INTEGRATION_CANNOTFINDSYMBOL);
		types.push_back(clss);
	}
}

void Efn::integration_param(Scope* scope) {
	GYuint i, s;

	outer=scope;
	s=parameters.size();
	for (i=0; i<s; i++) {
		parameters[i]->integration(scope);
		if (parameters[i]->getCallbyref())
			parameters[i]->setVariable(registerVariableRef(parameters[i]->getName()));
		else
			parameters[i]->setVariable(registerVariable(parameters[i]->getName()));
	}
}

void Efn::integration(Scope* scope) {
	integration_param(scope);
	if (body!=0) body->integration(this);
	if (condition!=0) condition->integration(this);
}

void Efns::integration(Scope* scope) {
	GYuint i, s;

	outer=scope;
	s=getLength();
	for (i=0; i<s; i++)
		at(i)->integration(this);
}

void Eclass::integration(Scope* scope) {
	GYuint i, s;
	Eclass *clss;

	outer=scope;
	s=supersname.size();
	for (i=0; i<s; i++) {
		clss=scope->findClass(supersname[i]);
		if (clss==0) error(ERROR_INTEGRATION_CANNOTFINDSYMBOL);
		supers.push_back(clss);
		registerUse(clss);
	}
	Eblock::integration(scope);			// 상위 클래스 integeration이 끝나면 이 클래스 내용도 integration
	// 근데 이거 어디 다른 데서 해주는 것 같기도 하고
}

void Use::integration(Scope* scope) {
	GYuint i, s;

	ref=scope->findClass(refpath[0]);
	s=refpath.size();
	for (i=1; i<s; i++) {
		ref=ref->findClass(refpath[i]);
		if (ref==0) error(ERROR_INTEGRATION_CANNOTFINDSYMBOL);
	}
	scope->registerUse(ref);
}

void Evar::integration(Scope* scope) {
	GYset<GYstring>::iterator it, end;

	it=vars.begin();
	end=vars.end();
	while (it!=end) {
		scope->registerVariable(*it);
		it++;
	}
}
