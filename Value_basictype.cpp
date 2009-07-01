#include "Types.h"
#include "Value_basictype.h"

Vbool::Vbool(Runner* runner) : Vinstance(Tbool::instance()->run(runner)) {
	value=false;
	values[type->findVariable("..value..")]=this;
}

Vbool::Vbool(Runner* runner, bool v) : Vinstance(Tbool::instance()->run(runner)) {
	value=v;
	values[type->findVariable("..value..")]=this;
}

Vint::Vint(Runner* runner) : Vinstance(Tint::instance()->run(runner)) {
	value=0;
	values[type->findVariable("..value..")]=this;
}

Vint::Vint(Runner* runner, int v) : Vinstance(Tint::instance()->run(runner)) {
	value=v;
	values[type->findVariable("..value..")]=this;
}

Vfloat::Vfloat(Runner* runner) : Vinstance(Tfloat::instance()->run(runner)) {
	value=0;
	values[type->findVariable("..value..")]=this;
}

Vfloat::Vfloat(Runner* runner, float v) : Vinstance(Tfloat::instance()->run(runner)) {
	value=v;
	values[type->findVariable("..value..")]=this;
}

Vstr::Vstr(Runner* runner) : Vinstance(Tstr::instance()->run(runner)) {
	value="";
	values[type->findVariable("..value..")]=this;
}

Vstr::Vstr(Runner* runner, GYstring v) : Vinstance(Tstr::instance()->run(runner)) {
	value=v;
	values[type->findVariable("..value..")]=this;
}

Vlist::Vlist(Runner* runner) : Vinstance(Tlist::instance()->run(runner)) {
	values[type->findVariable("..value..")]=this;
}
