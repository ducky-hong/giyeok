#pragma once

#include "basictypes.h"
#include "Element.h"
#include "Scope.h"
#include "Value.h"

class Variable;
class Runner;

class FNparam {
public:
	FNparam() { callbyref=false; vararg=false; }
	GYstring getName() { return name; }
	void setName(GYstring n) { name=n; }
	void addType(GYstring s) { typenames.push_back(s); }
	void setCallbyref(bool r) { callbyref=r; }
	bool getCallbyref() { return callbyref; }
	void setVariable(Variable *var) { variable=var; }
	Variable* getVariable() { return variable; }
	void setVararg(bool v) { vararg=v; }
	bool getVararg() { return vararg; }
	GYarray<Eclass*>* getTypes() { return &types; }
	GYarray<GYstring>* getTypeNames() { return &typenames; }
public:
	void integration(Scope*);
private:
	GYstring name;
	Variable* variable;
	GYarray<GYstring> typenames;
	GYarray<Eclass*> types;
	bool callbyref;
	bool vararg;
};

class Efn : public Element, public Scope {
public:
	Efn();
public:
	bool isAnonymous() { return anonym; }
	bool hasCondition() { return condition!=0; }
	Element* getCondition() { return condition; }
	GYstring getName() { return name; }
	void setName(GYstring n) { anonym=false; name=n; }
	void setCalltype(CallType ct) { calltype=ct; }
	CallType getCalltype() { return calltype; }
	FNparam* addParam();
	GYarray<FNparam*>& getParameters() { return parameters; }
	void setCondition(Element *c) { condition=c; }
	void setBody(Element *b) { body=b; }
	Element* getBody() { return body; }
	void setHasVararg(bool v) { hasvararg=v; }
	bool getHasVararg() { return hasvararg; }
public:
	Efn* clone();
	virtual void integration(Scope*);
	Vfn* run(Runner*);
	virtual Value* call(Runner* runner) { return body->run(runner); }
protected:
	void integration_param(Scope*);
protected:
	bool anonym;
	GYstring name;
	CallType calltype;
	GYarray<FNparam*> parameters;
	bool hasvararg;
	Element* condition;
	Element* body;
};

class Efns : public Element, public Scope {
public:
	void addFunction(Efn* f) { list.push_back(f); }
	void addFunction(Efns* f) {
		GYuint i, s;
		s=f->getLength();
		for (i=0; i<s; i++) list.push_back(f->at(i));
	}
	Efn* at(GYuint i) { return list.at(i); }
	GYuint getLength() { return list.size(); }
	GYarray<Efn*>* getListPointer() { return &list; }
public:
	Efns* clone();
	void integration(Scope*);
	Vfns* run(Runner*);
private:
	GYarray<Efn*> list;
};
