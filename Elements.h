#pragma once

#include "basictypes.h"
#include "Element.h"
#include "Scope.h"
#include "Value.h"

class Runner;

class Eif : public Element {
public:
	Eif(Element* c, Element* t, Element* e) {
		condition=c;
		then=t;
		elsethen=e;
	}
public:
	Eif* clone();
	void integration(Scope*);
	Value* run(Runner*);
protected:
	Element* condition;
	Element* then;
	Element* elsethen;
};

class Ewhile : public Element {
public:
	Ewhile(Element* c, Element* b) {
		condition=c;
		body=b;
	}
public:
	Ewhile* clone();
	void integration(Scope*);
	Value* run(Runner*);
protected:
	Element* condition;
	Element* body;
};

class Efor : public Element, public Scope {
public:
	Efor(GYstring v, Element* l, Element* b) {
		varname=v;
		list=l;
		body=b;
	}
	Element* getList() { return list; }
	Element* getBody() { return body; }
public:
	Efor* clone();
	void integration(Scope*);
	Value* run(Runner*);
protected:
	GYstring varname;
	Variable* variable;
	Element* list;
	Element* body;
};

class Eblock : public Element, public Scope {
public:
	GYarray<Element*>* getBodyPointer() { return &body; }
	void push_back(Element* e) { body.push_back(e); }
public:
	Eblock* clone();
	Eblock* clone(Eblock*);
	void integration(Scope*);
	Value* run(Runner*);
protected:
	GYarray<Element*> body;
};

class Eassign : public Element {
public:
	Eassign(GYstring t, Element* b) {
		targetname=t;
		body=b;
	}
public:
	Eassign* clone();
	void integration(Scope*);
	Value* run(Runner*);				// 0À» ¹ÝÈ¯
protected:
	GYstring targetname;
	Variable* target;
	Element* body;
};

class Ereturn : public Element {
public:
	Ereturn(Element* b) {
		body=b;
	}
public:
	Ereturn* clone();
	void integration(Scope*);
	Value* run(Runner*);
protected:
	Element* body;
};

class Evar : public Element {
public:
	void registerVariable(GYstring v) { vars.insert(v); }
public:
	Evar* clone();
	void integration(Scope*);
	Value* run(Runner*) { return 0; }
private:
	GYset<GYstring> vars;
};
