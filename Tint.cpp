#include "Types.h"
#include "Value_basictype.h"
#include "Runner.h"

Tint::Tint() : BasicType("int") {
	CustomFn *fn;
	FNparam *p;

	Scope::registerVariable("..value..");

	fn=new CustomFn();
	fn->setName("__add__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__add__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__sub__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__sub__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__mul__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__mul__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__div__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__div__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__divquot__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__divquot__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__divmod__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__divmod__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__equalto__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__equalto__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__lessthan__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__lessthan__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__lessthanequal__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__lessthanequal__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__greaterthan__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__greaterthan__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__greaterthanequal__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__greaterthanequal__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__and__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__and__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__or__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__or__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__xor__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tint::__xor__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__neg__");
	fn->setBody(Tint::__neg__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__complement__");
	fn->setBody(Tint::__complement__);
	registerFunction(fn);

	inst=this;
}

Value* Tint::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Vint *temp;

	switch (prms->size()) {
	case 0: return new Vint(runner);
	case 1:
		temp=castVint(prms->at(0));
		return new Vint(runner, temp->getValue());
	}
	;				// 오류 발생
	return 0;
}

Vint* Tint::castVint(Value* v) {
	return dynamic_cast<Vint*>(v->unwrap());
}

Value* Tint::__add__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())+(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__sub__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())-(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__mul__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())*(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__div__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vfloat *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vfloat(runner);
	r->setValue(((float)(v1->getValue()))/(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__divquot__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())/(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__divmod__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())%(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__equalto__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vbool *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())==(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__lessthan__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vbool *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())<(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__lessthanequal__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vbool *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())<=(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__greaterthan__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vbool *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())>(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__greaterthanequal__(Runner* runner, Efn* fni) {
	Vint *v1, *v2;
	Vbool *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())>=(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__and__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())&(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__or__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())|(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__xor__(Runner* runner, Efn* fni) {
	Vint *v1, *v2, *r;

	v1=castVint(fni->findVariable("..value..")->getValue());
	v2=castVint(fni->findVariable("a0")->getValue());
	r=new Vint(runner);
	r->setValue((v1->getValue())^(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__neg__(Runner* runner, Efn* fni) {
	Vint *v, *r;

	v=castVint(fni->findVariable("..value..")->getValue());
	r=new Vint(runner);
	r->setValue(-(v->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tint::__complement__(Runner* runner, Efn* fni) {
	Vint *v, *r;

	v=castVint(fni->findVariable("..value..")->getValue());
	r=new Vint(runner);
	r->setValue(~(v->getValue()));
	runner->registerValue(r);
	return r;
}
