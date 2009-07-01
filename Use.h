#pragma once

#include "basictypes.h"
#include "Element.h"

class Eclass;

class Use : public Element {
public:
	Use() {}
	void addRefPath(GYstring n) { refpath.push_back(n); }
public:
	Use* clone();
	void integration(Scope* s);
	Value* run(Runner*) { return 0; }
protected:
	GYarray<GYstring> refpath;
	Eclass* ref;
};
