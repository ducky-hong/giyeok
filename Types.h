#pragma once

#include "Eclass.h"
#include "Efn.h"

#include "BasicType.h"

class Scope;
class Runner;
class Value;
class Vbool;
class Vint;
class Vfloat;
class Vstr;
class Vlist;

class Tbool : public BasicType {
	static Tbool* inst;
public:
	Tbool();
	static Tbool* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Value* __not__(Runner*, Efn*);
	static Value* __and__(Runner*, Efn*);
	static Value* __or__(Runner*, Efn*);
	static Value* __xor__(Runner*, Efn*);
	static Value* __equalto__(Runner*, Efn*);
public:
	static Vbool* castVbool(Value*);
};

class Tint : public BasicType {
	static Tint* inst;
public:
	Tint();
	static Tint* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Value* __add__(Runner*, Efn*);
	static Value* __sub__(Runner*, Efn*);
	static Value* __mul__(Runner*, Efn*);
	static Value* __div__(Runner*, Efn*);
	static Value* __divquot__(Runner*, Efn*);
	static Value* __divmod__(Runner*, Efn*);
	static Value* __equalto__(Runner*, Efn*);
	static Value* __lessthan__(Runner*, Efn*);
	static Value* __lessthanequal__(Runner*, Efn*);
	static Value* __greaterthan__(Runner*, Efn*);
	static Value* __greaterthanequal__(Runner*, Efn*);
	static Value* __and__(Runner*, Efn*);
	static Value* __or__(Runner*, Efn*);
	static Value* __xor__(Runner*, Efn*);
	static Value* __neg__(Runner*, Efn*);
	static Value* __complement__(Runner*, Efn*);
public:
	static Vint* castVint(Value*);
};

class Tfloat : public BasicType {
	static Tfloat* inst;
public:
	Tfloat();
	static Tfloat* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Value* __add__(Runner*, Efn*);
	static Value* __sub__(Runner*, Efn*);
	static Value* __mul__(Runner*, Efn*);
	static Value* __div__(Runner*, Efn*);
	static Value* __equalto__(Runner*, Efn*);
	static Value* __lessthan__(Runner*, Efn*);
	static Value* __greaterthan__(Runner*, Efn*);
	static Value* __neg__(Runner*, Efn*);
public:
	static Vfloat* castVfloat(Value*);
};

class Tstr : public BasicType {
	static Tstr* inst;
public:
	Tstr();
	static Tstr* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Value* __append__(Runner*, Efn*);
	static Value* __empty__(Runner*, Efn*);
	static Value* __length__(Runner*, Efn*);
	static Value* __substr__(Runner*, Efn*);
public:
	static Vstr* castVstr(Value*);
};

class Tlist : public BasicType {
	static Tlist* inst;
public:
	Tlist();
	static Tlist* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Value* __append__(Runner*, Efn*);
	static Value* __insert__(Runner*, Efn*);
	static Value* __pop__(Runner*, Efn*);
	static Value* __set__(Runner*, Efn*);
	static Value* __at__(Runner*, Efn*);
	static Value* __size__(Runner*, Efn*);
public:
	static Vlist* castVlist(Value*);
};

class Tfn : public BasicType {
	static Tfn* inst;
public:
	Tfn();
	static Tfn* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
};

class Tclass : public BasicType {
	static Tclass* inst;
public:
	Tclass();
	static Tclass* instance() { return inst; }
public:
	Value* call(CallType, GYarray<Value*>*, Runner*);
public:
	static Vclass* castVclass(Value*);
};
