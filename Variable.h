#pragma once

class Value;

class Variable {
public:
	virtual void setValue(Value* v) = 0;
	virtual Value* getValue() = 0;
};

class VariableValue : public Variable {
public:
	VariableValue();
public:
	void setValue(Value* v);
	Value* getValue();
public:
	Value* value;
};

class VariableReference : public Variable {
public:
	VariableReference();
public:
	void setVariable(Variable *v);
	void setValue(Value* v);
	Value* getValue();
protected:
	Variable* value;
};
