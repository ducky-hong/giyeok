#include "Types.h"
#include "Value_basictype.h"
#include "Scope.h"

Tlist::Tlist() : BasicType("list") {
	CustomFn *fn;
	FNparam *p;

	Scope::registerVariable("..value..");

	fn=new CustomFn();
	fn->setName("__append__");
	p=fn->addParam();
	p->setName("a0");
	fn->setBody(Tlist::__append__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__insert__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	p=fn->addParam();
	p->setName("a1");
	fn->setBody(Tlist::__insert__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__pop__");
	fn->setBody(Tlist::__pop__);
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__set__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	p=fn->addParam();
	p->setName("a1");
	fn->setBody(Tlist::__set__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__at__");
	p=fn->addParam();
	p->setName("a0");
	p->addType("int");
	fn->setBody(Tlist::__at__);
	registerFunction(fn);

	fn=new CustomFn();
	fn->setName("__size__");
	fn->setBody(Tlist::__size__);
	registerFunction(fn);

	inst=this;
}

Value* Tlist::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	unsigned int i, s;
	Vlist *temp;

	temp=new Vlist(runner);
	s=prms->size();
	for (i=0; i<s; i++)
		temp->push_back(prms->at(i));
	return temp;
}

Vlist* Tlist::castVlist(Value* v) {
	return dynamic_cast<Vlist*>(v->unwrap());
}

Value* Tlist::__append__(Runner* runner, Efn* fni) {
	Vlist *v;
	Value *val;

	v=castVlist(fni->findVariable("..value..")->getValue());
	val=fni->findVariable("a0")->getValue()->unwrap();
	v->push_back(val);
	return v;
}

Value* Tlist::__insert__(Runner* runner, Efn* fni) {
	Vlist *v;
	Vint *idx;
	Value *val;

	v=castVlist(fni->findVariable("..value..")->getValue());
	idx=Tint::castVint(fni->findVariable("a0")->getValue());
	val=fni->findVariable("a1")->getValue()->unwrap();
	v->insert(v->begin()+idx->getValue(), val);
	return v;
}

Value* Tlist::__pop__(Runner* runner, Efn* fni) {
	Vlist *v;
	Vint *idx;
	Value *val;

	v=castVlist(fni->findVariable("..value..")->getValue());
	idx=Tint::castVint(fni->findVariable("a0")->getValue());
	val=v->at(idx->getValue());
	v->erase(v->begin()+idx->getValue());
	return val;
}

Value* Tlist::__set__(Runner* runner, Efn* fni) {
	Vlist *v=0;
	Vint *idx;
	Value *n;

	v=castVlist(fni->findVariable("..value..")->getValue());
	idx=Tint::castVint(fni->findVariable("a0")->getValue());
	n=fni->findVariable("a1")->getValue()->unwrap();
	if (idx!=0) v->at(idx->getValue())=n;
	return v;
}

Value* Tlist::__at__(Runner* runner, Efn* fni) {
	Vlist *v=0;
	Vint *idx;

	v=castVlist(fni->findVariable("..value..")->getValue());
	idx=Tint::castVint(fni->findVariable("a0")->getValue());
	if (idx!=0) return v->at(idx->getValue());
	return 0;
}

Value* Tlist::__size__(Runner* runner, Efn* fni) {
	Vlist *v=0;
	Vint *size;

	v=castVlist(fni->findVariable("..value..")->getValue());
	size=new Vint(runner, v->size());
	return size;
}
