#include <string>
#include <vector>

using namespace std;

typedef union literal{
    int i;
    uint ui;
    char c;
    bool b;
    string s;
}literal;

class ASTProg;
class ASTStmt;
class ASTNode;
class ASTBlock;
class ASTExpr;
class ASTVar;
class ASTValue;
class ASTVarDeclStmt;
class ASTVarAssgnStmt;
class ASTInputStmt;
class ASTOutputStmt;
class ASTUnaryExpr;
class ASTBinaryExpr;
class ASTCondExpr;
class ASTIfelseStmt;
class ASTWhileStmt;
class ASTBreakStmt;
class ASTContinueStmt;
class ASTReturnStmt;
class ASTFuncDeclStmt;
class ASTFuncCallStmt;
class ASTForStmt;

class ASTvisitor {
public:
	bool rhsIsVar = false;
    virtual antlrcpp::Any visit(ASTNode *node) = 0;
    virtual antlrcpp::Any visit(ASTProg *node) = 0;
    virtual antlrcpp::Any visit(ASTStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTBlock *node) = 0;
	virtual antlrcpp::Any visit(ASTExpr *node) = 0;
    virtual antlrcpp::Any visit(ASTValue *node) = 0;
    virtual antlrcpp::Any visit(ASTVar *node) = 0;
    virtual antlrcpp::Any visitForLHS(ASTVar *node) = 0;
    virtual antlrcpp::Any visitToAlloc(ASTVar *node, bool global=false) = 0;
    virtual antlrcpp::Any visit(ASTVarDeclStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTVarAssgnStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTInputStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTOutputStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTUnaryExpr *node) = 0;
    virtual antlrcpp::Any visit(ASTBinaryExpr *node) = 0;
    virtual antlrcpp::Any visit(ASTCondExpr *node) = 0;
    virtual antlrcpp::Any visit(ASTIfelseStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTWhileStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTBreakStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTContinueStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTReturnStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTFuncDeclStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTFuncCallStmt *node) = 0;
    virtual antlrcpp::Any visit(ASTForStmt *node) = 0;
};

class ASTnode {
public:
    virtual ~ASTnode() {
    }

    virtual antlrcpp::Any accept(ASTvisitor *V) = 0;
};

class ASTBlock : public ASTnode {
public:
	std::vector<ASTStmt*> stmtList;
	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }
};

class ASTProg : public ASTnode {
public:
    ASTBlock *mainBlock;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }
};


class ASTStmt : public ASTnode {
public:
	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }	
};

class ASTExpr : public ASTnode {
public:
	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }	
};

class ASTUnaryExpr : public ASTExpr {
public:
    string un_op;
    ASTExpr *expr;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTBinaryExpr : public ASTExpr {
public:
    string op;
    ASTExpr *left, *right;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTCondExpr : public ASTExpr {
public:
    ASTExpr *cond, *left, *right;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTVar : public ASTExpr {
public:
	string type, name;
	int dim;
	ASTExpr *r, *c;

	ASTVar(){		
	}

	ASTVar(int d, ASTExpr *rows, ASTExpr *cols, string n, string t){
		dim = d;
		r = rows;
		c = cols;
		name = n;
		type = t;
	}

	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }

    virtual antlrcpp::Any acceptForLHS(ASTvisitor *v) {
        return v->visitForLHS(this);
    }

	virtual antlrcpp::Any acceptToAlloc(ASTvisitor *v, bool global=false) {
        return v->visitToAlloc(this, global);
    }    
};

class ASTValue : public ASTExpr {
public:
    string type;
    string val;
    string getType(){
    	return this->type;
    }
	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }
};

class ASTVarDeclStmt : public ASTStmt {
public:
	std::vector<ASTVar*> vars;
	virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }	
};

class ASTVarAssgnStmt : public ASTStmt {
public:
    ASTVar *lhs;
    ASTExpr *rhs;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }   
};

class ASTInputStmt : public ASTStmt {
public:
    std::vector<ASTVar*> vars;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }   
};

class ASTOutputStmt : public ASTStmt {
public:
    std::vector<ASTExpr*> outs;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }   
};

class ASTIfelseStmt : public ASTStmt {
public:
    std::vector<ASTExpr*> conds;
    std::vector<ASTBlock*> blocks;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }   
};

class ASTWhileStmt : public ASTStmt {
public:
    ASTExpr *cond;
    ASTBlock *block;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }   
};

class ASTBreakStmt : public ASTStmt {
public:
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTContinueStmt : public ASTStmt {
public:
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTReturnStmt : public ASTStmt {
public:
    ASTExpr *expr;
    ASTReturnStmt(){
        expr = nullptr;
    }
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTFuncDeclStmt : public ASTStmt {
public:
    string ret_type, name;
    std::vector<ASTVar*> params;
    ASTBlock *block;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTForStmt : public ASTStmt {
public:
	vector<ASTVarAssgnStmt*> init, update;
    ASTExpr *cond;
    ASTBlock *block;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};

class ASTFuncCallStmt : public ASTStmt, public ASTExpr {
public:
    string name;
    std::vector<ASTExpr*> params;
    virtual antlrcpp::Any accept(ASTvisitor *v) {
        return v->visit(this);
    }  
};


