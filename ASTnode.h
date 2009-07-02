enum ASTNODETYPE {
	NODE_OPERATOR,
	NODE_ELEMENT,
	NODE_UNCALL,
	NODE_BINCALL,
	NODE_FNCALL,
	NODE_CLASSCALL,
	NODE_VARIABLECALL,
	NODE_VALUECALL,
	NODE_ANONYMCALL,
	NODE_DOT
};

class ASTNode {
public:
	virtual ASTNODETYPE getType()=0;
	virtual PieceType getObjType()=0;
	virtual Value* run(Runner* runner)=0;
};

class Noperator : public ASTNode {
public:
	Noperator(Operator* o) { opr=o; }
	virtual ASTNODETYPE getType() { return NODE_OPERATOR; }
	virtual PieceType getObjType() { return PT_OPERATOR; }
	Operator* getOperator() { return opr; }
	Value* run(Runner* runner) {
		return 0;
	}
public:
	Operator* getOpr() { return opr; }
protected:
	Operator *opr;
};

class Nelement : public ASTNode {
public:
	Nelement(Piece* p) { pc=p; }
	virtual ASTNODETYPE getType() { return NODE_ELEMENT; }
	virtual PieceType getObjType() { return pc->getType(); }
	Piece* getPiece() { return pc; }
	Value* run(Runner* runner);
protected:
	Piece *pc;
};

class Nbincall : public ASTNode {
public:
	Nbincall(Operator *o, ASTNode* t1, ASTNode* t2) { op=o; tar1=t1; tar2=t2; }
	virtual ASTNODETYPE getType() { return NODE_BINCALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Operator* getOpr() { return op; }
	ASTNode* getTar1() { return tar1; }
	ASTNode* getTar2() { return tar2; }
protected:
	Operator *op;
	ASTNode *tar1, *tar2;
};

class Nuncall : public ASTNode {
public:
	Nuncall(Operator *o, ASTNode* t1) { op=o; tar1=t1; }
	virtual ASTNODETYPE getType() { return NODE_UNCALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Operator* getOpr() { return op; }
	ASTNode* getTar1() { return tar1; }
protected:
	Operator *op;
	ASTNode *tar1;
};

class Nfncall : public ASTNode {
public:
	Nfncall(Efns* fs, Pcall* p) { fns=fs; prms=p; }
	virtual ASTNODETYPE getType() { return NODE_FNCALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Efns* getFns() { return fns; }
	Pcall* getPrms() { return prms; }
protected:
	Efns *fns;
	Pcall *prms;
};

class Nclasscall : public ASTNode {
public:
	Nclasscall(Eclass* c, Pcall* p) { clss=c; prms=p; }
	virtual ASTNODETYPE getType() { return NODE_CLASSCALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Eclass* getClass() { return clss; }
	Pcall* getPrms() { return prms; }
protected:
	Eclass *clss;
	Pcall *prms;
};

class Nvariablecall : public ASTNode {
public:
	Nvariablecall(Variable* v, Pcall* p) { var=v; prms=p; }
	virtual ASTNODETYPE getType() { return NODE_VARIABLECALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Variable* getVar() { return var; }
	Pcall* getPrms() { return prms; }
protected:
	Variable *var;
	Pcall *prms;
};

class Nvaluecall : public ASTNode {
public:
	Nvaluecall(ASTNode *n, Pcall* p) { node=n; prms=p; }
	virtual ASTNODETYPE getType() { return NODE_VALUECALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	ASTNode* getVal() { return node; }
	Pcall* getPrms() { return prms; }
protected:
	ASTNode *node;
	Pcall *prms;
};

class Ndot : public ASTNode {
public:
	Ndot(ASTNode* n, GYstring nm) { node=n; name=nm; }
	virtual ASTNODETYPE getType() { return NODE_DOT; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	void push(Runner*);
	void pop(Runner*);
public:
	ASTNode* getNode() { return node; }
	GYstring getName() { return name; }
protected:
	ASTNode *node;
	GYstring name;
private:
	ValueScope *inst;
	Vclass *clss;
	GYmap<Variable*, Value*> values;
};

class Nanonymcall : public ASTNode {
public:
	Nanonymcall(Ndot* o, Pcall* p) { obj=o; prms=p; }
	virtual ASTNODETYPE getType() { return NODE_ANONYMCALL; }
	virtual PieceType getObjType() { return PT_UNKNOWN; }
	Value* run(Runner* runner);
public:
	Ndot* getObj() { return obj; }
	Pcall* getPrms() { return prms; }
protected:
	Ndot *obj;
	Pcall *prms;
};
