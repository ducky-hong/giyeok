#include "Heap.h"

Heap::~Heap() {
	// values 완전히 제거
}

void Heap::addValue(Value* value) {
	values.insert(value);
	refcount.insert(GYpair<Value*, GYuint>(value,0));
}

void Heap::incRefCount(Value* value) {
//	(refcount.find(value)->second)++;
}

void Heap::decRefCount(Value* value) {
//	(refcount.find(value)->second)--;
}

GYuint Heap::getRefCount(Value* value) {
	int count;
	
	count=refcount.find(value)->second;
	if (count<0) return 0;
	return count;
}

void Heap::garbageCollect() {
	GYmap<Value*, int>::iterator it;

	for (it=refcount.begin(); it!=refcount.end(); it++)
		if (it->second<=0)
			values.erase(it->first);
}
