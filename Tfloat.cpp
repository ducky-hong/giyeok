#include "Types.h"
#include "Value_basictype.h"
#include "Runner.h"

Tfloat::Tfloat() : BasicType("float") {
	CustomFn *fn;
	FNparam *p;

	Scope::registerVariable("..value..");

	fn=new CustomFn();
	fn->setName("__add__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__add__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__sub__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__sub__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__mul__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__mul__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__div__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__div__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__equalto__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__equalto__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__lessthan__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__lessthan__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__greaterthan__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tfloat::__greaterthan__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__neg__");
	fn->setBody(Tfloat::__neg__);
	registerFunction(fn);

	inst=this;
}

Value* Tfloat::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Vfloat *temp;

	switch (prms->size()) {
	case 0: return new Vfloat(runner);
	case 1:
		temp=castVfloat(prms->at(0));
		return new Vfloat(runner, temp->getValue());
	}
	;				// 오류 발생
	return 0;
}

Vfloat* Tfloat::castVfloat(Value* v) {
	return dynamic_cast<Vfloat*>(v->unwrap());
}

Value* Tfloat::__add__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2, *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vfloat(runner);
	r->setValue((v1->getValue())+(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__sub__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2, *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vfloat(runner);
	r->setValue((v1->getValue())-(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__mul__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2, *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vfloat(runner);
	r->setValue((v1->getValue())*(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__div__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2, *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vfloat(runner);
	r->setValue((v1->getValue())/(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__equalto__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2;
	Vbool *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())==(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__lessthan__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2;
	Vbool *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())<(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__greaterthan__(Runner* runner, Efn* fni) {
	Vfloat *v1, *v2;
	Vbool *r;

	v1=castVfloat(fni->findVariable("..value..")->getValue());
	v2=castVfloat(fni->findVariable("a0")->getValue());
	r=new Vbool(runner);
	r->setValue((v1->getValue())>(v2->getValue()));
	runner->registerValue(r);
	return r;
}

Value* Tfloat::__neg__(Runner* runner, Efn* fni) {
	Vfloat *v, *r;

	v=castVfloat(fni->findVariable("..value..")->getValue());
	r=new Vfloat(runner);
	r->setValue(-(v->getValue()));
	runner->registerValue(r);
	return r;
}
