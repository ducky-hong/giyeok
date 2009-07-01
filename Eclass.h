#pragma once

#include "basictypes.h"
#include "Scope.h"
#include "Elements.h"
#include "Efn.h"
#include "Value.h"

class Runner;

class Eclass : public Eblock {
public:
	Eclass() { outer=0; }
	void merge(Eclass*);			// 다른 Eclass클래스의 내용을 더한다
public:
	GYstring getName() { return name; }
	void setName(GYstring n) { name=n; }
	void addSuper(GYstring n) { supersname.push_back(n); }
	void addSuper(Eclass* s) { supers.push_back(s); }
	GYarray<GYstring>* getSupersNames() { return &supersname; }
	GYarray<Eclass*>* getSupers() { return &supers; }
	bool istypeof(Eclass*);
public:
	virtual Variable* findVariable(GYstring);
	virtual Efns* findFn(GYstring, bool chkout=true);
	virtual Efns* findFn(GYstring, CallType, bool chkout=true);
	virtual Operators* findOperators(GYstring, bool chkout=true);
	virtual Eclass* findClass(GYstring, bool chkout=true);
public:
	virtual Eclass* clone();
	virtual void integration(Scope*);
	Vclass* run(Runner*);
	Value* initrun(Runner*);
private:
	GYstring name;
	GYarray<GYstring> supersname;
	GYarray<Eclass*> supers;
	GYarray<GYstring> prvfnnames;
	GYarray<Efn*> prvfunctions;
	GYarray<GYstring> prvclassnames;
	GYarray<Eclass*> prvclasses;
};
