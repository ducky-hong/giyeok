#include "Element.h"
#include "Efn.h"
#include "Eclass.h"
#include "Operator.h"

void Eclass::merge(Eclass *target) {			// 다른 Eclass클래스의 내용을 더한다
	// this가 앞에 오고 target이 뒤로 오도록
	// 포인터만 가져올 것이 아니라 아예 복사를 해서 새로운 객체로 가지고 올 것
	GYarray<GYstring> *supers;
	int i, s;

	// 상위 클래스 합치기
	supers=target->getSupersNames();			// supersname으로 해야 될까?
	s=supers->size();
	for (i=0; i<s; i++)
		addSuper(supers->at(i));
	// Eblock::merge로 해서 본문도 합쳐야 되겠다
	Scope::merge(target);
}

bool Eclass::istypeof(Eclass* e) {
	GYuint i, s;

	if (this==e) return true;
	s=supers.size();
	for (i=0; i<s; i++)
		if (supers[i]->istypeof(e)) return true;
	return false;
}

// Eclass에서는 variable, fn, op, class를 찾을 때 상위클래스들도 고려해야 함
Variable* Eclass::findVariable(GYstring name) {
	return Scope::findVariable(name);
}

Efns* Eclass::findFn(GYstring name, bool chkout) {
	return Scope::findFn(name, chkout);
}

Efns* Eclass::findFn(GYstring name, CallType type, bool chkout) {
	return Scope::findFn(name, type, chkout);
}

Operators* Eclass::findOperators(GYstring name, bool chkout) {
	return Scope::findOperators(name, chkout);
}

Eclass* Eclass::findClass(GYstring name, bool chkout) {
	return Scope::findClass(name, chkout);
}
