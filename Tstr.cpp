#include "Types.h"
#include "Value_basictype.h"
#include "Runner.h"
#include "BasicIO.h"

Tstr::Tstr() : BasicType("str") {
	CustomFn *fn;
	FNparam *p;

	Scope::registerVariable("..value..");

	fn=new CustomFn();
	fn->setName("__append__");
	p=fn->addParam();
	p->setName("a0");
	fn->setBody(Tstr::__append__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__empty__");
	fn->setBody(Tstr::__empty__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__length__");
	fn->setBody(Tstr::__length__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__substr__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	p=fn->addParam();
	p->setName("a1");
	p->addType("int");
	fn->setBody(Tstr::__substr__);
	registerFunction(fn);

	inst=this;
}

Vstr* Tstr::castVstr(Value* v) {
	return dynamic_cast<Vstr*>(v->unwrap());
}

Value* Tstr::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Vstr *temp;

	switch (prms->size()) {
	case 0: return new Vstr(runner);
	case 1:
		temp=castVstr(prms->at(0));
		return new Vstr(runner, temp->getValue());
	}
	;				// 오류 발생
	return 0;
}

Value* Tstr::__append__(Runner* runner, Efn* fni) {
	Vstr *v1, *r;
	GYstring s;

	v1=castVstr(fni->findVariable("..value..")->getValue());
	s=printitem(fni->findVariable("a0")->getValue());
	r=new Vstr(runner);
	r->setValue((v1->getValue())+s);
	runner->registerValue(r);
	return r;
}

Value* Tstr::__empty__(Runner* runner, Efn* fni) {
	Vstr *v1;
	Vbool *r;

	v1=castVstr(fni->findVariable("..value..")->getValue());
	r=new Vbool(runner);
	r->setValue(v1->getValue().empty());
	runner->registerValue(r);
	return r;
}

Value* Tstr::__length__(Runner* runner, Efn* fni) {
	Vstr *v1;
	Vint *r;

	v1=castVstr(fni->findVariable("..value..")->getValue());
	r=new Vint(runner);
	r->setValue(v1->getValue().size());
	runner->registerValue(r);
	return r;
}

Value* Tstr::__substr__(Runner* runner, Efn* fni) {
	Vstr *v1, *r;
	Vint *a1, *a2;

	v1=castVstr(fni->findVariable("..value..")->getValue());
	a1=Tint::castVint(fni->findVariable("a0")->getValue());
	a2=Tint::castVint(fni->findVariable("a0")->getValue());
	r=new Vstr(runner);
	r->setValue(v1->getValue().substr(a1->getValue(), a2->getValue()));
	runner->registerValue(r);
	return r;
}
