#ifndef AST_H
#include "ast.h"
#define AST_H
#endif
#ifndef SYMBOL_TABLE_H
#include "symbolTable.h"
#define SYMBOL_TABLE_H
#endif

bool isInt(string typ){
	return (typ == "int");
}

bool isUint(string typ){
	return (typ == "uint");
}

bool isBool(string typ){
	return (typ == "bool");
}

bool isChar(string typ){
	return (typ == "char");
}

bool isAssignCompatible(string typ1, string typ2){
	if(typ1 == typ2)
		return 1;
	return 0;
}

bool isUnaryCompatible(string un_op, string typ){
	if(un_op == "!" ){
		if(isBool(typ))
			return 1;
		return 0;
	}
	else{
		if(isInt(typ))
			return 1;
		return 0;
	}
}

string isBinaryCompatible(string op, string typ1, string typ2){
	// no cross type compatibility

	// arithmetic only for integers
	if(op == "+" || op == "-" || op == "*" || op == "/" || op == "%"){
		if(isInt(typ1) && isInt(typ2))
			return typ1;
		if(isUint(typ1) && isUint(typ2))
			return typ1;
		return "";
	}
	// relational for all types
	else if(op == ">" || op == "<" || op == ">=" || op == "<=" || op == "==" || op == "!="){
		if(typ1 == typ2)
			return "bool";
		return "";
	}
	// bitwise only for boolean and int
	else if(op == "&" || op == "|"){
		if(isInt(typ1) && isInt(typ2))
			return typ1;
		if(isUint(typ1) && isUint(typ2))
			return typ1;
		if(isBool(typ1) && isBool(typ2))
			return typ1;
		return "";
	}
	// logical only for bool
	else{
		if(isBool(typ1) && isBool(typ2))
			return typ1;
		return "";
	}
}

string isConditionalCompatible(string typ1, string typ2, string typ3){
	if(!isBool(typ1) || typ2 != typ3)
		return "";
	return typ2;
}

class semanticVisitor: public ASTvisitor {
private:
	bool createInBlock;
	int cnt;
	symbolTable symbols;
public:

	semanticVisitor(){
		this->createInBlock = 1;
		this->cnt = 0;
	}

	virtual antlrcpp::Any visit(ASTNode *node){
		// std::cout << "NODE\n";
		return "";
	}
    virtual antlrcpp::Any visit(ASTProg *node){
    	// std::cout << "PROG\n";
    	node->mainBlock->accept(this);
    	return "";
    }
    virtual antlrcpp::Any visit(ASTStmt *node){
    	// std::cout << "STMT\n";
    	return "";
    }
    virtual antlrcpp::Any visit(ASTBlock *node){
    	// std::cout << "BLOCK\n";
    	if(this->createInBlock){
    		this->symbols.addNewScope(std::to_string(this->cnt));
    		this->cnt++;
    	}
    	this->createInBlock = 1;
    	
    	for(auto stmt: node->stmtList){
    		stmt->accept(this);
    	}
    	this->symbols.exitScope();
    	return "";
    }
	virtual antlrcpp::Any visit(ASTExpr *node){
		// std::cout << "EXPR\n";
		return "";
	}
    virtual antlrcpp::Any visit(ASTValue *node){
    	// std::cout << "ASTVALUE\n";
    	
    	return node->getType();;
    }
    virtual antlrcpp::Any visitForLHS(ASTVar *node){
    	return "";
    }
    virtual antlrcpp::Any visitToAlloc(ASTVar *node, bool global=false){
    	return "";
    }
    virtual antlrcpp::Any visit(ASTVar *node){
    	// std::cout << "VAR\n";

    	// query based on name
    	const row* var = this->symbols.findVar(node->name);

    	if(node->dim > 0){
			string typ = node->r->accept(this);

			//check if index is of type int
			if(!isInt(typ)){
				error("ERROR: 0th Index of " + (node->name) + " should be int");
			}
		}
		if(node->dim > 1){
			string typ = node->c->accept(this);

			//check if index is of type int
			if(!isInt(typ)){
				error("ERROR: 1st Index of " + (node->name) + " should be int");
			}
		}
   
    	return var->getType();
    }
    virtual antlrcpp::Any visit(ASTVarDeclStmt *node){
    	// std::cout << "VARDECL\n";
    	for(auto var: node->vars){
    		if(var->dim > 0){
    			string typ = var->r->accept(this);

    			//check if index is of type int
    			if(!isInt(typ)){
    				error("ERROR: 0th Index of " + (var->name) + " should be int");
    			}
    		}
    		if(var->dim > 1){
    			string typ = var->c->accept(this);

    			//check if index is of type int
    			if(!isInt(typ)){
    				error("ERROR: 1st Index of " + (var->name) + " should be int");
    			}
    		}

    		this->symbols.addNewVariable(var->name, var->type, var->dim);
    	}
    	return "";
    }
    virtual antlrcpp::Any visit(ASTVarAssgnStmt *node){
    	// std::cout << "ASTVARASSGNSTMT\n";
    	string typ1 = node->lhs->accept(this);
    	string typ2 = node->rhs->accept(this);

    	// check type compatibility
    	if(!isAssignCompatible(typ1, typ2)){
    		error("ERROR: Type mismatch in assignment of " + (node->lhs->name));
    	}

    	return "";
    }
    virtual antlrcpp::Any visit(ASTInputStmt *node){
    	// std::cout << "INPUTSTMT\n";
    	
    	for(auto var: node->vars){
    		var->accept(this);
    	}
    	return "";
    }
    virtual antlrcpp::Any visit(ASTOutputStmt *node){
    	// std::cout << "OUTPUTSTMT\n";
    	for(auto var: node->outs){
    		var->accept(this);
    	}	
    	return "";
    }
    virtual antlrcpp::Any visit(ASTUnaryExpr *node){
    	// std::cout << "UNEXP\n";
		string typ = node->expr->accept(this);

		//check if integer (or bool in case of !)
		if(!isUnaryCompatible(node->un_op, typ)){
			error("ERROR: Type of expression not compatible for unary operator " + node->un_op);
		}
		// pass on the same type
		return typ;
    }
    virtual antlrcpp::Any visit(ASTBinaryExpr *node){
    	// std::cout << "BINEXP\n";
    	string typ1 = node->left->accept(this);
    	string typ2 = node->right->accept(this);
    	
    	// type check and inference 
    	string typ_out = isBinaryCompatible(node->op, typ1, typ2);
    	if(typ_out == ""){
    		error("ERROR: " + typ1 + " and " + typ2 + " are not compatible for " + node->op);
    	}
    	// pass on the inferred type
    	return typ_out;
    }
    virtual antlrcpp::Any visit(ASTCondExpr *node){
    	// std::cout << "CONDEXP\n";
    	string typ1 = node->cond->accept(this);
    	string typ2 = node->left->accept(this);
    	string typ3 = node->right->accept(this);
    	
    	// first should be bool and the next two of same type
    	string typ_out = isConditionalCompatible(typ1, typ2, typ3);
    	if(typ_out == ""){
    		error("ERROR: First expression should be boolean and the rest two should be of the same type");
    	}
    	// pass on the type of the second/third
    	return typ_out;
    }
    virtual antlrcpp::Any visit(ASTIfelseStmt *node){
    	// std::cout << "IFELSE\n";
    	int i = 0;
    	for(; i < node->conds.size(); i++){
    		string typ = node->conds[i]->accept(this);
    		node->blocks[i]->accept(this);
	    	
	    	// check if the condition is of boolean type
	    	if(!isBool(typ))
	    		error("ERROR: The condition in if/elif should of type boolean");
    	}
    	if(i < node->blocks.size())
    		node->blocks[i]->accept(this);
    	return "";
    }
    virtual antlrcpp::Any visit(ASTWhileStmt *node){
    	// std::cout << "WHILE\n";
    	

    	this->createInBlock = 0;
    	this->symbols.addNewScope(".loop");
    	
    	string typ = node->cond->accept(this);
    	node->block->accept(this);

    	
    	//check if the condition is of type boolean
    	if(!isBool(typ))
    		error("ERROR: The condition in while should of type boolean");	
    	
    	return "";
    }
    virtual antlrcpp::Any visit(ASTBreakStmt *node){
    	// cout << "BREAK\n";
    	// check if in loop
		this->symbols.findEnclosingLoop();
		return "";
    }
    virtual antlrcpp::Any visit(ASTContinueStmt *node){
    	// cout << "CONTINUE\n";
    	// check if in loop
    	this->symbols.findEnclosingLoop();
    	return "";
    }
    virtual antlrcpp::Any visit(ASTReturnStmt *node){
    	// cout << "RETURN\n";
    	string typ1 = "";
    	if(node->expr != NULL)
    		typ1 = node->expr->accept(this).as<string>();
    	
    	//find if return is in function and also match return type
    	string typ2 = this->symbols.findEnclosingFunc();
    	if(!isAssignCompatible(typ1, typ2)){
    		error("ERROR: Expected return type " + typ2 + " , but got " + typ1);
    	}
    	return "";
    }
    virtual antlrcpp::Any visit(ASTFuncDeclStmt *node){
    	// std::cout << "FUNCDECL\n";
		vector<row> ps;
    	for(auto p: node->params){
    		row temp(p->name, p->type, p->dim);
    		ps.push_back(temp);
    	}
    	this->symbols.addNewFunction(node->name, node->ret_type, ps);
    	
    	this->createInBlock = 0;
    	this->symbols.addNewScope(node->name);

    	for(auto var: node->params){
    		if(var->dim > 0){
    			string typ = var->r->accept(this);

    			//check if index is of type int
    			if(!isInt(typ)){
    				error("ERROR: 0th Index of " + var->name + " should be int");
    			}
    		}
    		if(var->dim > 1){
    			string typ = var->c->accept(this);

    			//check if index is of type int
    			if(!isInt(typ)){
    				error("ERROR: 1st Index of " + var->name + " should be int");
    			}
    		}

    		this->symbols.addNewVariable(var->name, var->type, var->dim);
    	}

    	node->block->accept(this);
    	return "";

    }
    virtual antlrcpp::Any visit(ASTFuncCallStmt *node){
    	// std::cout << "FUNCCALL\n";
    	const func* decl = this->symbols.findFunc(node->name);
    	
    	//match params
    	vector<string> typs = decl->getTypes();
    	if(typs.size() != node->params.size()){
    		error("ERROR: Expected " + to_string(typs.size()) + " params, got " + (to_string(node->params.size())) + " params in call of function " + node->name);
    	}
    	else{
    		for(int i = 0; i < typs.size(); i++){
    			string expr_typ = node->params[i]->accept(this);
    			if(!isAssignCompatible(typs[i], expr_typ)){
    				error("ERROR: Expected parameter at position " + to_string(i) + " to be of type " + (typs[i]) + " , got type " + (expr_typ));
    			}
    		}
    	}

    	//return type
    	return decl->type;
    }
    virtual antlrcpp::Any visit(ASTForStmt *node){
    	// std::cout << "FOR\n";
    	this->createInBlock = 0;
    	this->symbols.addNewScope(".loop");

		for(auto assgn: node->init)
			assgn->accept(this);
		string typ = node->cond->accept(this);
		for(auto assgn: node->update)
			assgn->accept(this);
		node->block->accept(this);
		
		//check if condition is boolean
		if(!isBool(typ))
			error("ERROR: Condition in for loop should of type boolean");
		
		return "";
    }
};
