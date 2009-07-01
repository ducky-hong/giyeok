#pragma once

#include "basictypes.h"
#include "Value.h"

class Heap {
public:
	Heap() {}
	~Heap();
	void addValue(Value*);
	void incRefCount(Value*);
	void decRefCount(Value*);
	GYuint getRefCount(Value*);
	void garbageCollect();
protected:
	GYset<Value*> values;
	GYmap<Value*, int> refcount;
};
