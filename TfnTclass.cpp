#include "Types.h"
#include "Value_basictype.h"
#include "Runner.h"

Tfn::Tfn() : BasicType("fn") {
	Scope::registerVariable("..value..");

	inst=this;
}

Value* Tfn::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Value *v;
	Vfn *fn;
	Vfns *fns;

	v=findVariable("..value..")->getValue()->unwrap();
	fn=dynamic_cast<Vfn*>(v);
	fns=dynamic_cast<Vfns*>(v);
	if (fn!=0) return fn->call(prms, runner);
	if (fns!=0) return fns->call(calltype, prms, runner);
	return 0;			// exception?
}

Tclass::Tclass() : BasicType("class") {
	Scope::registerVariable("..value..");

	inst=this;
}

Value* Tclass::call(CallType calltype, GYarray<Value*>* prms, Runner* runner) {
	Vclass *temp;

	temp=castVclass(findVariable("..value..")->getValue());
	return temp->call(calltype, prms, runner);
}

Vclass* Tclass::castVclass(Value* v) {
	return dynamic_cast<Vclass*>(v->unwrap());
}
