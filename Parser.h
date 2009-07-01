#pragma once

#include "basictypes.h"
#include <sstream>
#include "Elements.h"
#include "Eexpression.h"
#include "Efn.h"
#include "Eclass.h"
#include "Use.h"
#include "Operator.h"
#include "tokens.h"

class Parser {
public:
	void addLibrary(GYstring);
	Eblock* parseFile(GYstring);
private:
	void newtoken();
	TokenType whattoken();
	bool tokenisidentifier();
private:
	Element* readElement();
	Eif* readIf();
	Ewhile* readWhile();
	Efor* readFor();
	Eexpression* readExpression();
	Eblock* readBlock();
	Eblock* readBlock(Eblock*);
	Efns* readSBlock();
	Efn* readFunction();
	Eclass* readClass();
	Eassign* readAssign();
	Ereturn* readReturn();
	Use* readUse();
	Operators* readOperator();
	Evar* readVar();
private:
	std::stringstream oss;
	bool reusetoken;
	GYstring otoken;
	GYstring token;
	GYuint linenum;
private:
	Eblock *program;
	void loadLibrary();
	GYarray<GYstring> library;
};
