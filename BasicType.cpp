#include "Elements.h"
#include "Efn.h"
#include "Eclass.h"
#include "Scope.h"
#include "BasicType.h"

BasicType::BasicType(GYstring name) {
	setName(name);
}

void BasicType::integration(Scope* scope) {
//	GYuint size=functions.size();
//	for (GYuint i=0; i<size; i++)
//		functions[i]->integration(scope);
	Eclass::integration(scope);
}
