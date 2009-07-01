#pragma once

#include "Value.h"

class Scope;
class Runner;

class Element {
public:
	virtual Element* clone()=0;
	virtual void integration(Scope*)=0;
	virtual Value* run(Runner*)=0;
};
