#include "Runner.h"
#include "Heap.h"

Runner::Runner() {
	heap=new Heap();
	returned=false;
	stacks=0;
}

void Runner::registerValue(Value* v) {
	heap->addValue(v);
}

void Runner::assignValue(Value* v) {
	heap->incRefCount(v);
}

void Runner::deassignValue(Value* v) {
	heap->decRefCount(v);
}

void Runner::pushScope(Value* v, GYuint stacks) {
	ValueScope *vs;

	vs=dynamic_cast<ValueScope*>(v);
	if (vs!=0) {
//		callstack[stacks]->push(new GYpair<Variable*, Value*>(
	}
}

void Runner::pushScope(Value* v) {
	callstack.push_back(new GYmap<Variable*, Value*>());
	pushScope(v, stacks);
	stacks++;
}

void Runner::popScope() {
	stacks--;
	callstack.back();			// 얘를 원래대로 되돌려야 함
	callstack.pop_back();
}
