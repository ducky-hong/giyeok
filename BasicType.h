#pragma once
#include "Variable.h"

class Runner;

class BasicType : public Eclass {
public:
	BasicType(GYstring);
	void integration(Scope* scope);
};

class CustomFn : public Efn {
public:
	CustomFn() { }
public:
	void setBody(Value* (*b)(Runner*, Efn*)) { body=b; }
	void integration(Scope* scope) {		// 나중에 써야지
		integration_param(scope);
	}
	Value* call(Runner* runner) { return body(runner, this); }
private:
	Value* (*body)(Runner*, Efn*);
};
