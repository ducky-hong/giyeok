#include "Value.h"

class Vbool : public Vinstance {
public:
	Vbool(Runner*);
	Vbool(Runner*, bool v);
public:
	bool getValue() { return value; }
	void setValue(bool v) { value=v; }
private:
	bool value;
};

class Vint : public Vinstance {
public:
	Vint(Runner*);
	Vint(Runner*, int v);
public:
	int getValue() { return value; }
	void setValue(int v) { value=v; }
private:
	int value;
};

class Vfloat : public Vinstance {
public:
	Vfloat(Runner*);
	Vfloat(Runner*, float v);
public:
	float getValue() { return value; }
	void setValue(float v) { value=v; }
private:
	float value;
};

class Vstr : public Vinstance {
public:
	Vstr(Runner*);
	Vstr(Runner*, GYstring v);
public:
	GYstring getValue() { return value; }
	void setValue(GYstring v) { value=v; }
private:
	GYstring value;
};

class Vlist : public Vinstance, public GYarray<Value*> {
public:
	Vlist(Runner*);
};
