#pragma once

#include "basictypes.h"

class Variable;
class Element;
class Efn;
class Efns;
class Eclass;
class Operators;
class Runner;
class Scope;

enum ValueType {
	VT_VALUE,
	VT_VALUESCOPE,
	VT_INSTANCE,
	VT_FUNCTION,
	VT_FUNCTIONS,
	VT_BLOCK,
	VT_VARIABLE,
	VT_CLASS,
};

class Value {
public:
	Value() {}
public:
	virtual ValueType getType() { return VT_VALUE; }
	virtual bool instanceof(Eclass*) { return false; }
	virtual Value* unwrap() { return this; }
};

class ValueScope : public Value {
public:
	ValueScope() {}
	ValueScope(Scope* s);
	void init(Scope* s);
public:
	virtual ValueType getType() { return VT_VALUESCOPE; }
	void push(Runner*);
	void pop(Runner*);
protected:
	GYmap<Variable*, Value*> values;
	Scope* scope;
};

class Vfn : public ValueScope {
public:
	Vfn(Efn *b);
	void initmore(Element*);
public:
	virtual ValueType getType() { return VT_FUNCTION; }
	bool instanceof(Eclass*);
	Efn* getBody() { return body; }
public:
	bool fit(CallType, GYarray<Value*>*);
	Value* call(GYarray<Value*>*, Runner*);
	Value* call(Runner*);
	void setParameters(GYarray<Value*>* prms);
protected:
	Efn* body;
};

class Vfns : public ValueScope {
public:
	Vfns(Efns*, Runner*);
	void run(Runner*);
public:
	virtual ValueType getType() { return VT_FUNCTIONS; }
	bool instanceof(Eclass*);
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
protected:
	Efns* body;
	GYarray<Vfn*> list;
};

class Vblock : public ValueScope {
public:
	virtual ValueType getType() { return VT_BLOCK; }
};

class Vvariable : public Value {
public:
	Vvariable(Variable *v) { variable=v; }
public:
	virtual ValueType getType() { return VT_VARIABLE; }
	bool instanceof(Eclass*);
public:
	Value* unwrap();
	Variable* getValue() { return variable; }
private:
	Variable *variable;
};

class Vclass : public Value {
public:
	Vclass(Eclass *b) { body=b; }
public:
	virtual ValueType getType() { return VT_CLASS; }
	bool instanceof(Eclass*);
public:
	Variable* findVariable(GYstring);
	Operators* findOperators(GYstring name);
	Efns* findFunction(GYstring name);
	Eclass* findClass(GYstring name);
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	Eclass* getBody();
protected:
	Eclass* body;
};

class Vinstance : public ValueScope {
public:
	Vinstance() {}
	Vinstance(Vclass* t);
public:
	virtual ValueType getType() { return VT_INSTANCE; }
	Vclass* getClass() { return type; }
	bool instanceof(Eclass*);
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
protected:
	Vclass* type;
};
