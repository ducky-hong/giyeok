#pragma once

#include "basictypes.h"

class Variable;
class Efn;
class Efns;
class Operator;
class Operators;
class Eclass;
class Use;

class Scope {
public:
	Scope();
	void merge(Scope*);
	virtual Scope* getOuter();
public:
	Variable* registerVariable(GYstring);
	Variable* registerVariableRef(GYstring);
	void registerFunction(Efn*);
	void registerOperator(Operator*);
	void registerOperators(Operators*);
	void registerClass(Eclass*);
	void registerUse(Use*);
	void registerUse(Eclass*);
public:
	virtual Variable* findVariable(GYstring);
	virtual Efns* findFn(GYstring, bool chkout=true);
	virtual Efns* findFn(GYstring, CallType, bool chkout=true);
	virtual Operators* findOperators(GYstring, bool chkout=true);
	virtual Eclass* findClass(GYstring, bool chkout=true);
public:
	GYmap<GYstring, Variable*>::iterator getVariableIterator();
	GYmap<GYstring, Variable*>::iterator getVariableEndIterator();
	GYmap<GYstring, Operators*>::iterator getOperatorIterator();
	GYmap<GYstring, Operators*>::iterator getOperatorEndIterator();
	GYarray<Efn*>* getFnListPointer();
	GYmap<GYstring, Eclass*>::iterator getClassIterator();
	GYmap<GYstring, Eclass*>::iterator getClassEndIterator();
protected:
	GYmap<GYstring, Variable*> variables;
	GYarray<Efn*> functions;
	GYmap<GYstring, Operators*> operators;
	GYmap<GYstring, Eclass*> classes;
	GYarray<Use*> uses;
	GYarray<Eclass*> usingclasses;
	Scope* outer;
};
