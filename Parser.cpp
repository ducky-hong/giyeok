#include "Parser.h"
#include "Types.h"
#include "BasicIO.h"
#include <fstream>
#include <iostream>

Tbool* Tbool::inst;
Tint* Tint::inst;
Tfloat* Tfloat::inst;
Tstr* Tstr::inst;
Tlist* Tlist::inst;
Tfn* Tfn::inst;
Tclass* Tclass::inst;

void Parser::addLibrary(GYstring filepath) {
	// filepath을 라이브러리로 사용할 것을 등록해 놓고
	library.push_back(filepath);
}

void Parser::loadLibrary() {
	GYuint i, s;

	s=library.size();
	for (i=0; i<s; i++) {
		std::ifstream ifs(library[i].c_str());
		oss.clear();
		oss << ifs.rdbuf();
		program=readBlock(program);
		ifs.close();
	}
}

Eblock* Parser::parseFile(GYstring filepath) {
	std::ifstream ifs(filepath.c_str());

	reusetoken=false;
	if(!ifs) return false;
	program=new Eblock();
	program->registerClass(new Tbool());
	program->registerClass(new Tint());
	program->registerClass(new Tfloat());
	program->registerClass(new Tstr());
	program->registerClass(new Tlist());
	program->registerClass(new Tfn());
	program->registerClass(new Tclass());
	registerBasicIOfunctions(program);
	linenum=1;
	loadLibrary();
	oss.clear();
	oss << ifs.rdbuf();
	if(!ifs && !ifs.eof()) return false;
	program=readBlock(program);
	ifs.close();
	program->integration(0);
	return program;
}
