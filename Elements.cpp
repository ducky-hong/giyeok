#include "Element.h"
#include "Efn.h"

FNparam* Efn::addParam() {
	FNparam *param;

	param=new FNparam();
	parameters.push_back(param);
	return param;
}

Efn::Efn() {
	anonym=true;
	hasvararg=false;
	condition=0;
	body=0;
	calltype=CT_ROUND;
}
