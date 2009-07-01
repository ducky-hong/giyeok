#include "Scope.h"
#include "Efn.h"
#include "Eclass.h"
#include "BasicType.h"
#include "Types.h"
#include "BasicIO.h"
#include "Value_basictype.h"
#include <iostream>
#include <sstream>

GYstring printitem(Value* value) {
	Vstr *vs;
	Vfloat *vf;
	Vint *vi;
	Vbool *vb;
	Vlist *vl;
	std::stringstream ss;

	ss.clear();
	value=value->unwrap();
	vs=dynamic_cast<Vstr*>(value);
	vf=dynamic_cast<Vfloat*>(value);
	vi=dynamic_cast<Vint*>(value);
	vb=dynamic_cast<Vbool*>(value);
	vl=dynamic_cast<Vlist*>(value);
	if (vs!=0) ss<<vs->getValue();
	else if (vf!=0) ss<<vf->getValue();
	else if (vi!=0) ss<<vi->getValue();
	else if (vb!=0) ss<<(vb->getValue()? "true":"false");
	else if (vl!=0) {
		GYuint i, s;
		s=vl->size();
		ss<<"[";
		for (i=0; i<s; i++) {
			if (i>0) ss<<";";
			ss<<printitem(vl->at(i));
		}
		ss<<"]";
	}
//	if (i+1<s) std::cout<<" ";
	return ss.str();
}

Value* basicio__print__(Runner* runner, Efn* fni) {
	Value *value;

	value=fni->findVariable("a0")->getValue();
	std::cout<<printitem(value);
	return 0;
}

Value* basicio__println__(Runner* runner, Efn* fni) {
	basicio__print__(runner, fni);
	std::cout<<std::endl;
	return 0;
}

void registerBasicIOfunctions(Scope* program) {
	CustomFn *fn;
	FNparam *p;

	fn=new CustomFn();
	fn->setName("print");
	fn->setCalltype(CT_ROUND);
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	p->addType("int");
	p->addType("float");
	p->addType("str");
	p->addType("list");
	fn->setBody(basicio__print__);
	program->registerFunction(fn);

	fn=new CustomFn();
	fn->setName("println");
	fn->setCalltype(CT_ROUND);
	p=fn->addParam();
	p->setName("a0");
	p->addType("bool");
	p->addType("int");
	p->addType("float");
	p->addType("str");
	p->addType("list");
	fn->setBody(basicio__println__);
	program->registerFunction(fn);
}
