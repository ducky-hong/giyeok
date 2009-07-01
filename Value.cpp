#include "Value.h"
#include "Element.h"
#include "Eclass.h"
#include "Efn.h"
#include "Runner.h"
#include "Variable.h"
#include "Types.h"
#include "Value_basictype.h"
#include "error.h"

ValueScope::ValueScope(Scope* s) {
	// s에 있는 변수들을 values에 등록한다
	// values.insert(GYpair<Variable*, Value*>(type->findVariable("..value.."), this));
	init(s);
}

void ValueScope::init(Scope* s) {
	if (s!=0) {
		GYmap<GYstring, Variable*>::iterator it, end;
//		init(s->getOuter());
		it=s->getVariableIterator();
		end=s->getVariableEndIterator();
		while (it!=end) {
			if (dynamic_cast<VariableValue*>(it->second)!=0) {
				values.insert(GYpair<Variable*, Value*>(it->second, 0));
				values[it->second]=it->second->getValue();
			}
			it++;
		}
	}
}

void ValueScope::push(Runner* runner) {
	// 현재 갖고 있는 값들을 변수에 집어 넣는다
	GYmap<Variable*, Value*>::iterator it, end;
	Value *temp;

	it=values.begin();
	end=values.end();
	while (it!=end) {
		temp=it->second;
		values[it->first]=it->first->getValue();
		it->first->setValue(temp);
		it++;
	}
}

void ValueScope::pop(Runner* runner) {
	// 변수에 있는 값들을 저장한다
	GYmap<Variable*, Value*>::iterator it, end;
	Value *temp;

	it=values.begin();
	end=values.end();
	while (it!=end) {
		temp=it->first->getValue();
		it->first->setValue(values[it->first]);
		values[it->first]=temp;
		it++;
	}
}

Vfn::Vfn(Efn *b) : ValueScope(b) {
	body=b;
	// body가 스코프인 경우 value에 추가해줘야 함
	// for, block, fn, class
	initmore(body->getBody());
}

void Vfn::initmore(Element *e) {
	Efor *fr;
	Eblock *block;
	Efn *fn;
	Eclass *clss;

	if (e==0) return;
	fr=dynamic_cast<Efor*>(e);
	block=dynamic_cast<Eblock*>(e);
	fn=dynamic_cast<Efn*>(e);
	clss=dynamic_cast<Eclass*>(e);
	if (fr!=0) {
		init(fr);
		initmore(fr->getList());
		initmore(fr->getBody());
	}
	if (block!=0) {
		GYarray<Element*> *body;
		GYuint i, s;

		init(block);
		body=block->getBodyPointer();
		s=body->size();
		for (i=0; i<s; i++)
			if (dynamic_cast<Scope*>(body->at(i))!=0)
				initmore(body->at(i));
	}
	if (fn!=0) {
		init(fn);
		initmore(fn->getBody());
	}
	if (clss!=0) {
		init(clss);
	}
}

Vfns::Vfns(Efns *b, Runner* runner) : ValueScope(b) {
	body=b;
	run(runner);
}

void Vfns::run(Runner* runner) {
	GYarray<Efn*> *l;
	GYuint i, s;

	l=body->getListPointer();
	s=l->size();
	for (i=0; i<s; i++)
		list.push_back(l->at(i)->run(runner));
}

bool Vfn::fit(CallType calltype, GYarray<Value*>* prms) {
	// 주어진 파라메터의 형태가 이 함수에 맞으면 true 아니면 false
	// 가변크기 인자도 고려
	GYarray<FNparam*> &parameters=body->getParameters();
	if (calltype!=body->getCalltype()) return false;
	if (body->getHasVararg()) {
		parameters.size();
		if (prms->size()<parameters.size()) return false;
		return true;
	} else {
		GYuint i, s, j, js;
		bool f;

		if (prms->size()!=parameters.size()) return false;
		s=parameters.size();
		for (i=0; i<s; i++) {
			js=parameters[i]->getTypes()->size();
			if (js>0) {
				f=false;
				for (j=0; j<js; j++) {
					// prms->at(i)가 parameters[i]->getTypes()->at(j)인지 확인
					if (prms->at(i)->instanceof(parameters[i]->getTypes()->at(j))) {
						f=true;
						break;
					}
				}
				if (!f) return false;
			}
		}
		return true;
	}
}

Value* Vfn::call(GYarray<Value*>* prms, Runner* runner) {
	Value *result;

	push(runner);
	setParameters(prms);
	result=body->call(runner);
	if (result!=0) result=result->unwrap();
	pop(runner);
	return result;
}

Value* Vfn::call(Runner* runner) {
	Value *result;
	result=body->call(runner);
	if (result!=0) result=result->unwrap();
	return result;
}

void Vfn::setParameters(GYarray<Value*>* prms) {
	GYuint i, s;
	Variable *var;
	Vvariable *vvar;
	GYarray<FNparam*> &parameters=body->getParameters();

	s=prms->size();
	for (i=0; i<s; i++) {
		var=parameters[i]->getVariable();
		vvar=dynamic_cast<Vvariable*>(prms->at(i));
		if (parameters[i]->getCallbyref()) {
			// Call by reference일 경우
			if (vvar==0)
				error(ERROR_RUNTIME_PARAMNEEDSVARIABLE);		// 오류 - call by reference에는 변수가 가야 함!
			dynamic_cast<VariableReference*>(var)->setVariable(vvar->getValue());
		} else {
			// Call by value일 경우
			if (vvar!=0) var->setValue(vvar->getValue()->getValue());
			else var->setValue(prms->at(i));
		}
	}
}

/*
Value* Vfn::call(GYarray<Element*>* prms, Runner* runner) {
	GYarray<Value*>* rprms;
	GYuint i, s;
	
	s=prms->size();
	rprms=new GYarray<Value*>();
	for (i=0; i<s; i++)
		rprms->push_back(prms->at(i)->run(runner));
	return call(rprms, runner);
}
*/
Value* Vfns::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	GYuint i, s;
	Value *result=0, *cond;
	Vbool *condb;
	
	s=list.size();
	for (i=0; i<s; i++)
		if (list[i]->fit(calltype, prms)) {
			if (list[i]->getBody()->hasCondition()) {
				list[i]->push(runner);
				list[i]->setParameters(prms);
				cond=list[i]->getBody()->getCondition()->run(runner);
				condb=dynamic_cast<Vbool*>(cond);
				if (condb!=0 && condb->getValue()) {
					result=list[i]->call(runner);
					list[i]->pop(runner);
				} else {
					list[i]->pop(runner);
					continue;
				}
				break;
			} else {
				result=list[i]->call(prms, runner);
				break;
			}
		}
	return result;
}
/*
Value* Vfns::call(CallType calltype, GYarray<Element*>* prms, Runner* runner) {
	GYarray<Value*>* rprms;
	GYuint i, s;
	
	s=prms->size();
	rprms=new GYarray<Value*>();
	for (i=0; i<s; i++)
		rprms->push_back(prms->at(i)->run(runner));
	return call(calltype, rprms, runner);
}
*/
bool Vfn::instanceof(Eclass* e) {
	return e->istypeof(Tfn::instance());
}

bool Vfns::instanceof(Eclass* e) {
	return e->istypeof(Tfn::instance());
}

bool Vvariable::instanceof(Eclass* e) {
	return variable->getValue()->instanceof(e);
}

Value* Vvariable::unwrap() {
	return variable->getValue();
}

bool Vclass::instanceof(Eclass* e) {
	return e->istypeof(Tclass::instance());
}

Value* Vclass::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	// 새로 생성된 인스턴스를 runner에 추가하고 반환한다
	Vinstance *inst;
	Efns *fns;
	
	// 새 인스턴스를 만들어서 runner에 넣고 스택에 push한 뒤
	inst=new Vinstance(this);
	inst->push(runner);
	// 클래스 본문을 실행하고
	body->initrun(runner);
	// 적당한 anonymous 함수를 찾아서 실행하고
	fns=body->findFn("", calltype, false);
	if (fns!=0) fns->run(runner)->call(calltype, prms, runner);
	// 스택에서 pop한 뒤에 반환
	inst->pop(runner);
	return inst;
}

Variable* Vclass::findVariable(GYstring name) {
	return body->findVariable(name);
}

Operators* Vclass::findOperators(GYstring name) {
	return body->findOperators(name);
}

Efns* Vclass::findFunction(GYstring name) {
	Efns *fns=body->findFn(name);

	if (fns->getLength()==0) { delete fns; fns=0; }
	return fns;
}

Eclass* Vclass::findClass(GYstring name) {
	Eclass *clss=body->findClass(name);

	return clss;
}

Eclass* Vclass::getBody() {
	return body;
}

Vinstance::Vinstance(Vclass* t) : ValueScope(t->getBody()) {
	type=t;
}

Value* Vinstance::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Value *result;
	Efns *fns;

//	runner->pushScope(this);
	push(runner);
	fns=type->findFunction("");
	if (fns!=0) {
		result=fns->run(runner)->call(calltype, prms, runner);
		result=result->unwrap();
	}
	pop(runner);
//	runner->popScope();
	return result;
}

bool Vinstance::instanceof(Eclass* e) {
	return type->getBody()->istypeof(e);
}
