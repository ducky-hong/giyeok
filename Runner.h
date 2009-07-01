#pragma once

#include "Runner.h"
#include "Value.h"

class Heap;

class Runner {
public:
	Runner();
public:
	void registerValue(Value*);
	void assignValue(Value*);
	void deassignValue(Value*);
public:
	void pushScope(Value*);
	void popScope();
public:
	void incReturned() { returned++; }
	void decReturned() { returned--; }
	bool getReturned() { return returned>0; }
private:
	void pushScope(Value*, GYuint);
private:
	Heap *heap;
	GYuint returned;
private:
	GYarray<GYmap<Variable*, Value*>*> callstack;
	GYuint stacks;
};
