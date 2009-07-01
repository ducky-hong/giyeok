#pragma once

#include "basictypes.h"
#include "Element.h"

enum OperatorType {
	INFIXL,
	INFIXR,
	PREFIX
};

class Operator {
public:
	Operator() {}
	Operator(OperatorType t, GYuchar p, GYstring n) {
		type=t; priority=p; name=n;
	}
	Operator* clone();
public:
	OperatorType getOperatorType() { return type; }
	GYuchar getPriority() { return priority; }
	GYstring getName() { return name; }
	void setFns(Efns *fs) { fns=fs; }
	Efns* getFns() { return fns; }
private:
	OperatorType type;
	GYuchar priority;
	GYstring name;
	Efns* fns;
};

class Operators : public Element {
public:
	Operators() { }
public:
	void merge(Operators *ops) {
		GYuint i, s;
		
		if (ops!=0) {
			s=ops->size();
			for (i=0; i<s; i++) add(ops->at(i));
		}
	}
	void add(Operator *op) { list.push_back(op); }
	GYuint size() { return list.size(); }
	Operator* at(GYuint i) { return list.at(i); }
	Operator* findinfix(bool infix) {
		// infix가 true이면 infix 형태의 연산자를 반환
		// false이면 prefix 형태의 연산자를 반환
		if (infix) {
			for (GYuint i=list.size()-1; i>0; i--)
				if (list[i]->getOperatorType()==INFIXL || list[i]->getOperatorType()==INFIXR)
					return list[i];
			if (list[0]->getOperatorType()==INFIXL || list[0]->getOperatorType()==INFIXR)
				return list[0];
		} else {
			for (GYuint i=list.size()-1; i>0; i--)
				if (list[i]->getOperatorType()==PREFIX)
					return list[i];
			if (list[0]->getOperatorType()==PREFIX)
				return list[0];
		}
		return 0;
	}
public:
	Operators* clone();
	void integration(Scope*) {}
	Value* run(Runner*) { return 0; }
private:
	GYarray<Operator*> list;
};
