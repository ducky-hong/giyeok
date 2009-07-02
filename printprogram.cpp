#include <fstream>
#include "Element.h"
#include "Elements.h"
#include "Efn.h"
#include "Eclass.h"
#include "Eexpression.h"
#include "ASTnode.h"
#include "Scope.h"
using namespace std;

void printelement(ofstream&, Element*, int);

GYstring ind(int indent) {
	GYstring s;

	s.clear();
	s.append(indent*4, ' ');
	return s;
}

void printscope(ofstream &file, Scope* scope, int indent) {
	GYmap<GYstring, Eclass*>::iterator itc, endc;
	GYarray<Efn*>* fns;
	GYmap<GYstring, Operators*>::iterator ito, endo;
	GYmap<GYstring, Variable*>::iterator itv, endv;
	GYuint i, s;

	itc=scope->getClassIterator();
	endc=scope->getClassEndIterator();
	while (itc!=endc) {
		printelement(file, itc->second, indent);
		itc++;
	}
	fns=scope->getFnListPointer();
	s=fns->size();
	for (i=0; i<s; i++)
		printelement(file, fns->at(i), indent);
	ito=scope->getOperatorIterator();
	endo=scope->getOperatorEndIterator();
	while (ito!=endo) {
		file << ind(indent) << "operator " << ito->first << endl;
		ito++;
	}
	itv=scope->getVariableIterator();
	endv=scope->getVariableEndIterator();
	while (itv!=endv) {
		file << ind(indent) << "variable " << itv->first << "#" << itv->second << endl;
		itv++;
	}
}

void printpcall(ofstream &file, Pcall *pcall, int indent) {
	GYarray<Element*> *list;
	GYuint i, s;

	file << ind(indent);
	if (pcall->getCallType()==CT_ROUND) file << "(" << endl;
	else file << "[" << endl;
	list=pcall->getListPointer();
	s=list->size();
	for (i=0; i<s; i++)
		printelement(file, list->at(i), indent);
	file << ind(indent);
	if (pcall->getCallType()==CT_ROUND) file << ")" << endl;
	else file << "]" << endl;
}

void printastnode(ofstream &file, ASTNode* node, int indent) {
	Noperator *nop;
	Nelement *nelem;
	Nbincall *nbin;
	Nuncall *nun;
	Nfncall *nfn;
	Nclasscall *nclss;
	Nvariablecall *nvar;
	Nvaluecall *nval;
	Ndot *ndot;
	Nanonymcall *nanonym;

	switch (node->getType()) {
	case NODE_OPERATOR:
		nop=dynamic_cast<Noperator*>(node);
		file << ind(indent) << "operator" << nop->getOperator()->getName() << endl;
		break;
	case NODE_ELEMENT:
		nelem=dynamic_cast<Nelement*>(node);
		file << ind(indent);
		switch (nelem->getObjType()) {
		case PT_BOOL:
			file << "bool " << ((dynamic_cast<Pbool*>(nelem->getPiece())->getValue())? "true":"false") << endl;
			break;
		case PT_INT:
			file << "int " << dynamic_cast<Pint*>(nelem->getPiece())->getValue() << endl;
			break;
		case PT_FLOAT:
			file << "float " << dynamic_cast<Pfloat*>(nelem->getPiece())->getValue() << endl;
			break;
		case PT_STR:
			file << "str \"" << dynamic_cast<Pstr*>(nelem->getPiece())->getValue() << "\"" << endl;
			break;
		case PT_ELEMENT:
			file << "element" << endl;
			printelement(file, dynamic_cast<Pelement*>(nelem->getPiece())->getValue(), indent+1);
			break;
		case PT_FN:
			file << "float " << dynamic_cast<Pfns*>(nelem->getPiece())->getValue() << endl;
			break;
		case PT_CLASS: {
			Eclass *clss;
			
			clss=dynamic_cast<Pclass*>(nelem->getPiece())->getValue();
			file << "class " << clss->getName() << "#" << clss << endl;
			break;
					   }
		case PT_LIST: {
			GYarray<Element*> *el=dynamic_cast<Plist*>(nelem->getPiece())->getListPointer();
			GYuint i, s;

			file << "list [" << endl;
			s=el->size();
			for (i=0; i<s; i++)
				printelement(file, el->at(i), indent+1);
			file << ind(indent) << "]" << endl;
			break;
					  }
		case PT_VARIABLE:
			file << "variable #" << dynamic_cast<Pvariable*>(nelem->getPiece())->getValue() << endl;
		}
		break;
	case NODE_UNCALL:
		nun=dynamic_cast<Nuncall*>(node);
		file << ind(indent) << "uncall " << nun->getOpr()->getName() << endl;
		printastnode(file, nun->getTar1(), indent+1);
		break;
	case NODE_BINCALL:
		nbin=dynamic_cast<Nbincall*>(node);
		file << ind(indent) << "bincall " << nbin->getOpr()->getName() << endl;
		printastnode(file, nbin->getTar2(), indent+1);
		printastnode(file, nbin->getTar1(), indent+1);
		break;
	case NODE_FNCALL: {
		GYarray<Efn*> *list;
		GYuint i, s;

		nfn=dynamic_cast<Nfncall*>(node);
		file << ind(indent) << "fncall";
		list=nfn->getFns()->getListPointer();
		s=list->size();
		for (i=0; i<s; i++) {
			file << " " << list->at(i)->getName() << "#" << list->at(i);
		}
		file << endl;
		file << ind(indent) << "  parameters:" << endl;
		printpcall(file, nfn->getPrms(), indent+1);
		break;
					  }
	case NODE_CLASSCALL:
		nclss=dynamic_cast<Nclasscall*>(node);
		file << ind(indent) << "classcall " << nclss->getClass()->getName() << "#" << nclss->getClass() << endl;
		file << ind(indent) << "  parameters:" << endl;
		printpcall(file, nclss->getPrms(), indent+1);
		break;
	case NODE_VARIABLECALL:
		nvar=dynamic_cast<Nvariablecall*>(node);
		file << ind(indent) << "variablecall #" << nvar->getVar() << endl;
		file << ind(indent) << "  parameters:" << endl;
		printpcall(file, nvar->getPrms(), indent+1);
		break;
	case NODE_VALUECALL:
		nval=dynamic_cast<Nvaluecall*>(node);
		file << ind(indent) << "valuecall" << endl;
		file << ind(indent) << "  value:" << endl;
		printastnode(file, nval->getVal(), indent+1);
		file << ind(indent) << "  parameters:" << endl;
		printpcall(file, nval->getPrms(), indent+1);
		break;
	case NODE_ANONYMCALL:
		nanonym=dynamic_cast<Nanonymcall*>(node);
		file << ind(indent) << "anonymcall" << endl;
		file << ind(indent) << "  obj:" << endl;
		printastnode(file, nanonym->getObj(), indent+1);
		file << ind(indent) << "  parameters:" << endl;
		printpcall(file, nanonym->getPrms(), indent+1);
		break;
	case NODE_DOT:
		ndot=dynamic_cast<Ndot*>(node);
		file << ind(indent) << "dot" << endl;
		file << ind(indent) << "  obj:" << endl;
		printastnode(file, ndot->getNode(), indent+1);
		file << ind(indent) << "  name: " << ndot->getName() << endl;
		break;
	}
}

void printexpression(ofstream &file, Eexpression* expr, int indent) {
	printastnode(file, expr->getASTnode(), indent);
}

void printelement(ofstream &file, Element* elem, int indent) {
	Eif *eif;
	Ewhile *ewhile;
	Efor *efor;
	Eblock *eblock;
	Eassign *eassign;
	Ereturn *ereturn;
//	Evar *evar;
	Efn *efn;
	Efns *efns;
	Eclass *eclass;
	Eexpression *eexpr;

	eif=dynamic_cast<Eif*>(elem);
	ewhile=dynamic_cast<Ewhile*>(elem);
	efor=dynamic_cast<Efor*>(elem);
	eblock=dynamic_cast<Eblock*>(elem);
	eassign=dynamic_cast<Eassign*>(elem);
	ereturn=dynamic_cast<Ereturn*>(elem);
//	evar=dynamic_cast<Evar*>(elem);
	efn=dynamic_cast<Efn*>(elem);
	efns=dynamic_cast<Efns*>(elem);
	eclass=dynamic_cast<Eclass*>(elem);
	eexpr=dynamic_cast<Eexpression*>(elem);
	if (eif!=0) {
		file << ind(indent) << "if statement" << endl;
		file << ind(indent) << "condition:" << endl;
		printelement(file, eif->getCondition(), indent+1);
		file << ind(indent) << "then:" << endl;
		printelement(file, eif->getThen(), indent+1);
		if (eif->getElse()!=0) {
			file << ind(indent) << "else:" << endl;
			printelement(file, eif->getElse(), indent+1);
		}
	}
	if (ewhile!=0) {
		file << ind(indent) << "while statement" << endl;
		file << ind(indent) << "  condition:" << endl;
		printelement(file, ewhile->getCondition(), indent+1);
		file << ind(indent) << "  body:" << endl;
		printelement(file, ewhile->getBody(), indent+1);
	}
	if (efor!=0) {
		file << ind(indent) << "for statement (control variable " << efor->getVarName() << "#" << efor->getVariable() << ")" << endl;
		file << ind(indent) << "  list:" << endl;
		printelement(file, efor->getList(), indent+1);
		file << ind(indent) << "  body:" << endl;
		printelement(file, efor->getBody(), indent+1);
	}
	if (eclass!=0) {
		GYarray<GYstring> *supersname;
		GYarray<Eclass*> *supers;
		GYuint i, s;

		file << ind(indent) << "class " << eclass->getName() << endl;
		supersname=eclass->getSupersNames();
		s=supersname->size();
		if (s>0) {
			file << ind(indent) << "  supers:";
			supers=eclass->getSupers();
			for (i=0; i<s; i++)
				file << " " << supersname->at(i) << "#" << supers->at(i);
			file << endl;
		}
	}
	if (eblock!=0) {
		GYarray<Element*>* body;
		GYuint i, s;

		body=eblock->getBodyPointer();
		s=body->size();
		file << ind(indent) << "{" << endl;
		printscope(file, eblock, indent+1);
		file << ind(indent) << "  body:" << endl;
		for (i=0; i<s; i++)
			printelement(file, body->at(i), indent+1);
		file << ind(indent) << "}" << endl;
	}
	if (eassign!=0) {
		file << ind(indent) << "assginemt statement" << endl;
		file << ind(indent) << "  target: " << eassign->getTargetName() << "#" << eassign->getTarget() << endl;
		file << ind(indent) << "  value:" << endl;
		printelement(file, eassign->getBody(), indent+1);
	}
	if (ereturn!=0) {
		file << ind(indent) << "return statement" << endl;
		file << ind(indent) << "  value:" << endl;
		printelement(file, ereturn->getBody(), indent+1);
	}
	if (efn!=0) {
		GYarray<FNparam*> parameters;
		GYuint i, s, j, js;

		file << ind(indent) << "fn " << efn->getName() << " ";
		if (efn->getCalltype()==CT_ROUND) file << "("; else file <<"[";
		parameters=efn->getParameters();
		s=parameters.size();
		for (i=0; i<s; i++) {
			GYarray<GYstring> *typenames;
			GYarray<Eclass*> *types;

			if (i>0) file <<" ; ";
			typenames=parameters[i]->getTypeNames();
			types=parameters[i]->getTypes();
			js=typenames->size();
			for (j=0; j<js; j++)
				file << typenames->at(j) << "#" << types->at(j) << " ";
			file << parameters[i]->getName() << "#" << parameters[i]->getVariable();
		}
		if (efn->getCalltype()==CT_ROUND) file << ")" << endl; else file << "]" << endl;
		if (efn->getCondition()!=0) {
			file << ind(indent) << "  condition:" << endl;
			printelement(file, efn->getCondition(), indent+1);
		}
		file << ind(indent) << "  body:" << endl;
		printelement(file, efn->getBody(), indent+1);
	}
	if (efns!=0) {
		GYarray<Efn*> *list;
		GYuint i, s;

		file << ind(indent) << "function set" << endl;
		list=efns->getListPointer();
		s=list->size();
		for (i=0; i<s; i++)
			printelement(file, list->at(i), indent+1);
	}
	if (eexpr!=0) {
		printexpression(file, eexpr, indent);
	}
}

void printprogram(GYstring filename, Eblock* program) {
	GYstring newfilename;

	newfilename=filename;
	newfilename.append(".pp");
	ofstream file(newfilename.c_str());
	file << "    Program loaded from " << filename << endl;
	if (file.is_open()) {
		printelement(file, program, 0);
		file.close();
	}
}
