#include "Element.h"
#include "Eexpression.h"
#include "ASTnode.h"
#include "Efn.h"
#include "Eclass.h"
#include "Runner.h"
#include "Variable.h"
#include "Types.h"
#include "Value_basictype.h"

Value* Eif::run(Runner* runner) {
	Value *cond;
	Vbool *inbool;

	cond=condition->run(runner)->unwrap();
	inbool=dynamic_cast<Vbool*>(cond);
	if (inbool!=0) {
		if (inbool->getValue()) return then->run(runner);
		else if (elsethen!=0) return elsethen->run(runner);
	}
	return 0;
}

Value* Ewhile::run(Runner* runner) {
	Value *cond;
	Vbool *inbool;
	Vlist *result;

	result=new Vlist(runner);
	runner->registerValue(result);
	cond=condition->run(runner);
	inbool=dynamic_cast<Vbool*>(cond);
	if (inbool==0) return 0;
	while (inbool->getValue()) {
		result->push_back(body->run(runner));
		cond=condition->run(runner);
		inbool=dynamic_cast<Vbool*>(cond);
		if (inbool==0) return 0;
	}
	return result;
}

Value* Efor::run(Runner* runner) {
	Value *cand;
	Vlist *inlist;
	Vlist *result;
	GYuint i, s;

	result=new Vlist(runner);
	cand=list->run(runner)->unwrap();
	inlist=dynamic_cast<Vlist*>(cand);
	if (inlist==0) return 0;
	s=inlist->size();
	for (i=0; i<s; i++) {
		variable->setValue(inlist->at(i));
		cand=body->run(runner);
		if (cand!=0) cand=cand->unwrap();
		if (cand!=0) result->push_back(cand);
	}
	return result;
}

Value* Eexpression::run(Runner* runner) {
	if (ast==0) return 0;
	return ast->run(runner);
}

Value* Eblock::run(Runner* runner) {
	GYuint i, s;
	Value *value;
	GYmap<GYstring, Eclass*>::iterator it, end;

	it=classes.begin();
	end=classes.end();
	while (it!=end) {
		it->second->initrun(runner);
		it++;
	}
	s=body.size();
	for (i=0; i<s; i++) {
		value=body[i]->run(runner);
		if (runner->getReturned()) {
			runner->decReturned();
			if (value!=0) value=value->unwrap();
			return value;
		}
//		delete value;
	}
	return 0;
}

Value* Eassign::run(Runner* runner) {
	Value *value;

	value=body->run(runner);
	if (value==0) return 0;
	value=value->unwrap();
	if (target->getValue()!=0)
		runner->deassignValue(target->getValue());
	target->setValue(value);
	runner->assignValue(value);
	return 0;		// 0À» ¹İÈ¯
}

Value* Ereturn::run(Runner* runner) {
	Value *value;

	value=body->run(runner);
	runner->registerValue(value);
	runner->incReturned();
	return value;
}

Vfn* Efn::run(Runner* runner) {
	Vfn *result;
	
	result=new Vfn(this);
	return result;
}

Vfns* Efns::run(Runner* runner) {
	Vfns *result;
	
	result=new Vfns(this, runner);
	return result;
}

Vclass* Eclass::run(Runner* runner) {
	Vclass *result;
	
	result=new Vclass(this);
	return result;
}

Value* Eclass::initrun(Runner* runner) {
	return Eblock::run(runner);
}
