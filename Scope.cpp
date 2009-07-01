#include "Scope.h"
#include "Variable.h"
#include "Operator.h"
#include "Eclass.h"
#include "Use.h"

Scope::Scope() {
	outer=0;
}

void Scope::merge(Scope *target) {
	// 내용 합치기
	// integration 전에 호출된다
	GYuint i, s;
	GYmap<GYstring, Operators*>::iterator ito, endo;
	GYarray<Efn*> *fns;
	GYmap<GYstring, Eclass*>::iterator itc, endc;

	ito=target->getOperatorIterator();
	endo=target->getOperatorEndIterator();
	while (ito!=endo) {
		registerOperators(ito->second->clone());
		ito++;
	}
	fns=target->getFnListPointer();
	s=fns->size();
	for (i=0; i<s; i++) {
		Efn *fn=fns->at(i)->clone();
		registerFunction(fn);
	}
	itc=target->getClassIterator();
	endc=target->getClassEndIterator();
	while (itc!=endc) {
		Eclass *clss=itc->second->clone();
		registerClass(clss);
		itc++;
	}
}

Scope* Scope::getOuter() {
	return outer;
}

Variable* Scope::registerVariable(GYstring name) {
	Variable *var;

	var=new VariableValue();
	variables.insert(GYpair<GYstring, Variable*>(name, var));
	return var;
}

Variable* Scope::registerVariableRef(GYstring name) {
	Variable *var;

	var=new VariableReference();
	variables.insert(GYpair<GYstring, Variable*>(name, var));
	return var;
}

void Scope::registerFunction(Efn* fn) {
	functions.push_back(fn);
}

void Scope::registerOperator(Operator* op) {
	GYmap<GYstring, Operators*>::iterator it;

	it=operators.find(op->getName());
	if (it==operators.end())
		operators.insert(GYpair<GYstring, Operators*>(op->getName(), new Operators()));
	operators[op->getName()]->add(op);
}

void Scope::registerOperators(Operators* ops) {
	GYuint i, s;

	s=ops->size();
	for (i=0; i<s; i++)
		registerOperator(ops->at(i));
}

void Scope::registerClass(Eclass* clss) {
	GYmap<GYstring, Eclass*>::iterator fnd;
	GYstring name;

	name=clss->getName();
	fnd=classes.find(name);
	if (fnd==classes.end()) classes[name]=clss;
	else fnd->second->merge(clss);
}

void Scope::registerUse(Use* use) {
	uses.push_back(use);
}

void Scope::registerUse(Eclass* clss) {
	GYmap<GYstring, Eclass*>::iterator it, end, fnd;

	usingclasses.push_back(clss);
	// 변수, 함수, 클래스를 모두 복사해서 내것처럼 해 놓는다
/*	it=clss->getClassIterator();
	end=clss->getClassEndIterator();
	while (it!=end) {
		fnd=classes.find(it->first);
		if (fnd!=classes.end()) {
			fnd->second->merge(it->second);
		}
		it++;
	}*/
	merge(clss);
}

Variable* Scope::findVariable(GYstring name) {
	GYmap<GYstring, Variable*>::iterator fnd;

	fnd=variables.find(name);
	if (fnd==variables.end()) {
		if (outer==0) return 0;
		else return outer->findVariable(name);
	}
	return fnd->second;
}

// findFn, findOperator, findClass 할 때에는 use를 참조하지 않고

Efns* Scope::findFn(GYstring name, bool chkout) {
	GYuint i, s;
	Efns *fns;

	s=functions.size();
	fns=new Efns();
	for (i=0; i<s; i++) {
		if (functions[s-i-1]->getName()==name)
			fns->addFunction(functions[s-i-1]);
	}
//	s=usingclasses.size();
//	for (i=0; i<s; i++)
//		fns->addFunction(usingclasses[i]->findFn(name, false));
	if (chkout && outer!=0) fns->addFunction(outer->findFn(name));
	return fns;
}

Efns* Scope::findFn(GYstring name, CallType type, bool chkout) {
	GYuint i, s;
	Efns *fns;

	s=functions.size();
	fns=new Efns();
	for (i=s; i>0; i--) {
		if (functions[i-1]->getName()==name && functions[i-1]->getCalltype()==type)
			fns->addFunction(functions[i-1]);
	}
//	s=usingclasses.size();
//	for (i=s; i>0; i--)
//		fns->addFunction(usingclasses[i-1]->findFn(name, type, false));
	if (chkout && outer!=0) fns->addFunction(outer->findFn(name, type));
	return fns;
}

Operators* Scope::findOperators(GYstring name, bool chkout) {
	GYmap<GYstring, Operators*>::iterator fnd;
	Operators *ops;
//	GYuint i, s;

	ops=new Operators();
	fnd=operators.find(name);
//	s=usingclasses.size();
//	for (i=s; i>0; i--)
//		ops->merge(usingclasses[i-1]->findOperators(name, false));
	if (chkout && outer!=0) ops->merge(outer->findOperators(name));
	if (fnd!=operators.end()) ops->merge(fnd->second);
	if (ops->size()==0) { delete ops; return 0; }
	return ops;
}

Eclass* Scope::findClass(GYstring name, bool chkout) {
	GYmap<GYstring, Eclass*>::iterator fnd;

	fnd=classes.find(name);
	if (fnd==classes.end()) {
//		GYuint i, s;
//		Eclass* res;
//		s=usingclasses.size();
//		for (i=0; i<s; i++) {
//			res=usingclasses[i]->findClass(name, false);
//			if (res!=0) return res;
//		}
		if (chkout && outer!=0) return outer->findClass(name);
		else return 0;
	}
	return fnd->second;
}

GYmap<GYstring, Variable*>::iterator Scope::getVariableIterator() {
	return variables.begin();
}

GYmap<GYstring, Variable*>::iterator Scope::getVariableEndIterator() {
	return variables.end();
}

GYmap<GYstring, Operators*>::iterator Scope::getOperatorIterator() {
	return operators.begin();
}

GYmap<GYstring, Operators*>::iterator Scope::getOperatorEndIterator() {
	return operators.end();
}

GYarray<Efn*>* Scope::getFnListPointer() {
	return &functions;
}

GYmap<GYstring, Eclass*>::iterator Scope::getClassIterator() {
	return classes.begin();
}

GYmap<GYstring, Eclass*>::iterator Scope::getClassEndIterator() {
	return classes.end();
}
