#include "Variable.h"

VariableValue::VariableValue() {
}

void VariableValue::setValue(Value *v) {
	value=v;
}

Value* VariableValue::getValue() {
	return value;
}

VariableReference::VariableReference() {
}

void VariableReference::setVariable(Variable *v) {
	value=v;
}

void VariableReference::setValue(Value *v) {
	value->setValue(v);
}

Value* VariableReference::getValue() {
	return value->getValue();
}
