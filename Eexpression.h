#pragma once

#include "basictypes.h"
#include "Element.h"
#include "Value.h"
#include "Operator.h"

class Runner;

enum PieceType {
	PT_UNKNOWN,
	PT_BOOL,
	PT_INT,
	PT_FLOAT,
	PT_STR,
	PT_LIST,
	PT_ELEMENT,
	PT_DOT,
	PT_OPERATOR,
	PT_FN,
	PT_CLASS,
	PT_CALL,
	PT_VARIABLE,
	PT_IDENTIFIER
};

class Piece {
public:
	virtual PieceType getType()=0;
//	virtual Value* run()=0;
};

class Pbool : public Piece {
public:
	Pbool() {}
	Pbool(bool v) { value=v; }
public:
	PieceType getType() { return PT_BOOL; }
	bool getValue() { return value; }
private:
	bool value;
};

class Pint : public Piece {
public:
	Pint() {}
	Pint(int v) { value=v; }
	Pint(GYstring s) { value=atoi(s.c_str()); }
	int getValue() { return value; }
public:
	PieceType getType() { return PT_INT; }
private:
	int value;
};

class Pfloat : public Piece {
public:
	Pfloat() {}
	Pfloat(float v) { value=v; }
	Pfloat(GYstring s) { value=(float)atof(s.c_str()); }
	float getValue() { return value; }
public:
	PieceType getType() { return PT_FLOAT; }
private:
	float value;
};

class Pstr : public Piece {
public:
	Pstr() {}
	Pstr(GYstring v) { value=v.substr(1, v.size()-2); }
	GYstring getValue() { return value; }
public:
	PieceType getType() { return PT_STR; }
private:
	GYstring value;
};

class Plist : public Piece {
public:
	Plist() {}
	GYarray<Element*>* getListPointer() { return &list; }
public:
	PieceType getType() { return PT_LIST; }
private:
	GYarray<Element*> list;
};

class Pelement : public Piece {
public:
	Pelement(Element* v) { value=v; }
	Element* getValue() { return value; }
public:
	PieceType getType() { return PT_ELEMENT; }
private:
	Element* value;
};

class Pdot : public Piece {
public:
	Pdot() {}
public:
	PieceType getType() { return PT_DOT; }
};

class Poperators : public Piece {
public:
	Poperators(Operators *v) { value=v; }
	Operators* getValue() { return value; }
public:
	PieceType getType() { return PT_OPERATOR; }
private:
	Operators *value;
};

class Pfns : public Piece {
public:
	Pfns() {}
	Pfns(Efns *v) { value=v; }
	Efns* getValue() { return value; }
public:
	PieceType getType() { return PT_FN; }
private:
	Efns *value;
};

class Pclass : public Piece {
public:
	Pclass() {}
	Pclass(Eclass *v) { value=v; }
	Eclass* getValue() { return value; }
public:
	PieceType getType() { return PT_CLASS; }
private:
	Eclass *value;
};

class Pcall : public Piece {
public:
	Pcall(CallType t) { type=t; }
	GYarray<Element*>* getListPointer() { return &list; }
	GYarray<Value*>* run(Runner* runner) {
		GYarray<Value*> *rcall;
		GYuint i, s;

		rcall=new GYarray<Value*>();
		s=list.size();
		for (i=0; i<s; i++)
			rcall->push_back(list[i]->run(runner));
		return rcall;
	}
	CallType getCallType() { return type; }
public:
	PieceType getType() { return PT_CALL; }
private:
	CallType type;
	GYarray<Element*> list;
};

class Pvariable : public Piece {
public:
	Pvariable(Variable *v) { value=v; }
	Variable* getValue() { return value; }
public:
	PieceType getType() { return PT_VARIABLE; }
private:
	Variable *value;
};

class Pidentifier : public Piece {
public:
	Pidentifier(GYstring v) { value=v; }
public:
	PieceType getType() { return PT_IDENTIFIER; }
	GYstring getName() { return value; }
private:
	GYstring value;
};

class ASTNode;

class Eexpression : public Element {
public:
	GYarray<Piece*>* getBodyPointer() { return &body; }
	ASTNode* getASTNode() { return ast; }
public:
	Eexpression* clone();
	void clonePieces();
	Piece* clonePiece(Piece*);
	void integration(Scope*);
	Value* run(Runner*);
public:
	ASTNode* getASTnode() { return ast; }
protected:
	GYarray<Piece*> body;
	ASTNode *ast;
};
