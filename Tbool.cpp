#include "Types.h"
#include "Value_basictype.h"
#include "Runner.h"

Tbool::Tbool() : BasicType("bool") {
	CustomFn *fn;
	FNparam *p;

	Scope::registerVariable("..value..");

	fn=new CustomFn();
	fn->setName("__not__");
	fn->setBody(Tbool::__not__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__and__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	fn->setBody(Tbool::__and__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__or__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	fn->setBody(Tbool::__or__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__xor__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	fn->setBody(Tbool::__xor__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__equalto__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	fn->setBody(Tbool::__equalto__);
	registerFunction(fn);

	inst=this;
}

Value* Tbool::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Vbool *temp;

	switch (prms->size()) {
	case 0: return new Vbool(runner);
	case 1:
		temp=castVbool(prms->at(0));
		return new Vbool(runner, temp->getValue());
	}
	;				// 오류 발생
	return 0;
}

Vbool* Tbool::castVbool(Value* v) {
	return dynamic_cast<Vbool*>(v->unwrap());
}

Value* Tbool::__not__(Runner* runner, Efn* fni) {
	Vbool *v, *r;

	v=castVbool(fni->findVariable("..value..")->getValue());
	r=new Vbool(runner);
	r->setValue(!(v->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tbool::__and__(Runner* runner, Efn* fni) {
	Vbool *v1, *v2, *r;

	v1=castVbool(fni->findVariable("..value..")->getValue());
	v2=castVbool(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())&&(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tbool::__or__(Runner* runner, Efn* fni) {
	Vbool *v1, *v2, *r;

	v1=castVbool(fni->findVariable("..value..")->getValue());
	v2=castVbool(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())||(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tbool::__xor__(Runner* runner, Efn* fni) {
	Vbool *v1, *v2, *r;

	v1=castVbool(fni->findVariable("..value..")->getValue());
	v2=castVbool(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())!=(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tbool::__equalto__(Runner* runner, Efn* fni) {
	Vbool *v1, *v2, *r;

	v1=castVbool(fni->findVariable("..value..")->getValue());
	v2=castVbool(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())==(v2->getValue()));
	runner->registerValue(r);
	return r;
}
