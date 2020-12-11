#include <iostream>
#ifndef AST_H
#include "ast.h"
#define AST_H
#endif
#include <vector>
#include "miniCVisitor.h"

using namespace std;


class miniCBuildASTVisitor : public miniCVisitor {
public:

	virtual antlrcpp::Any visitProg(miniCParser::ProgContext *context){
		// cout << "Visting prog" << endl;
		ASTProg *node = new ASTProg();
		node->mainBlock = visit(context->block());
		return (ASTProg *) node;
	};

    virtual antlrcpp::Any visitStmt2var_decl(miniCParser::Stmt2var_declContext *context){
    	// cout << "Visting stmt2var_decl" << endl;
    	ASTVarDeclStmt *node = visit(context->var_decl());
    	return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2var_assgn(miniCParser::Stmt2var_assgnContext *context){
        // cout << "Visting stmt2var_assgn" << endl;
        ASTVarAssgnStmt *node = visit(context->var_assgn());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2ifelse(miniCParser::Stmt2ifelseContext *context){
        // cout << "Visiting ifelse" << endl;
        ASTIfelseStmt *node = new ASTIfelseStmt();
        for(auto conds : context->expr()){
            node->conds.push_back(visit(conds));
        }
        for(auto blocks : context->block()){
            node->blocks.push_back(visit(blocks));
        }

        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitMul_var_assgn(miniCParser::Mul_var_assgnContext *context){
    	// pass
        return NULL;
    }

    virtual antlrcpp::Any visitStmt2for(miniCParser::Stmt2forContext *context){
        // cout << "Visiting for" << endl;
        ASTForStmt *node = new ASTForStmt();
        for(auto var_assgn : context->mul_var_assgn()[0]->var_assgn()){
    		node->init.push_back(visit(var_assgn));
    	}
        node->cond = visit(context->expr());
    	for(auto var_assgn : context->mul_var_assgn()[1]->var_assgn()){
    		node->update.push_back(visit(var_assgn));
    	}
        node->block = visit(context->block());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2while(miniCParser::Stmt2whileContext *context){
        // cout << "Visiting while" << endl;
        ASTWhileStmt *node = new ASTWhileStmt();
        node->cond = visit(context->expr());
        node->block = visit(context->block());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2block(miniCParser::Stmt2blockContext *context){
        // cout << "Visiting stmt2block" << endl;
        ASTBlock *node = visit(context->block());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2break(miniCParser::Stmt2breakContext *context){
        // cout << "Visiting break" << endl;
        ASTBreakStmt *node = new ASTBreakStmt();
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2continue(miniCParser::Stmt2continueContext *context){
        // cout << "Visiting continue" << endl;
        ASTContinueStmt *node = new ASTContinueStmt();
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2return(miniCParser::Stmt2returnContext *context){
        // cout << "Visiting return" << endl;
        ASTReturnStmt *node = new ASTReturnStmt();
        if(context->expr() != nullptr){
            node->expr = visit(context->expr());
        }

        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2input(miniCParser::Stmt2inputContext *context){
        // cout << "Visiting stmt2input" << endl;
        ASTInputStmt *node = new ASTInputStmt();
        for(auto var : context->var()){
            node->vars.push_back(visit(var));
        }
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2output(miniCParser::Stmt2outputContext *context){
        // cout << "Visiting stmt2output" << endl;
        ASTOutputStmt *node = new ASTOutputStmt();
        for(auto expr : context->expr()){
            node->outs.push_back(visit(expr));
        }
        return (ASTStmt *) node;  
    };

    virtual antlrcpp::Any visitStmt2func_decl(miniCParser::Stmt2func_declContext *context){
        // cout << "Visiting stmt2funcdecl" << endl;
        ASTFuncDeclStmt *node = visit(context->func_decl());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitStmt2func_call(miniCParser::Stmt2func_callContext *context){
        // cout << "Visiting stmt2funccall" << endl;
        ASTFuncCallStmt *node = visit(context->func_call());
        return (ASTStmt *) node;
    };

    virtual antlrcpp::Any visitFunc_decl(miniCParser::Func_declContext *context){
        // cout << "Visiting funcdecl" << endl;
        ASTFuncDeclStmt *node = new ASTFuncDeclStmt();
        node->name = context->ID()->getText();
        node->ret_type = context->type()[0]->getText();
        for(int i = 1; i < context->type().size(); i += 1){
            ASTVar *var = visit(context->var()[i-1]);
            var->type = context->type()[i]->getText();
            node->params.push_back(var);
        }
        node->block = visit(context->block());
        
        return (ASTFuncDeclStmt *) node;
    };

    virtual antlrcpp::Any visitFunc_call(miniCParser::Func_callContext *context){
        // cout << "Visiting funccall" << endl;
        ASTFuncCallStmt *node = new ASTFuncCallStmt();
        node->name = context->ID()->getText();
        for(auto expr : context->expr()){
            node->params.push_back(visit(expr));
        }
        return (ASTFuncCallStmt *) node;
    };

    virtual antlrcpp::Any visitBlock(miniCParser::BlockContext *context){
    	// cout << "Visiting Block" << endl;
    	ASTBlock *node = new ASTBlock();
    	ASTStmt *stmtNode;

    	for(auto stmt : context->stmt()){
    		stmtNode = visit(stmt);
    		if(stmtNode != nullptr){
    			 node->stmtList.push_back(stmtNode);
    		}
    	}

    	return (ASTBlock * ) node;
    };

    virtual antlrcpp::Any visitVar_decl(miniCParser::Var_declContext *context){
        // cout << "Visting var_decl" << endl;
        ASTVarDeclStmt *node = new ASTVarDeclStmt();
        ASTVar *variable;
        string type = context->type()->getText();
        for(auto var : context->var()){
            variable = visit(var);
            variable->type = type;
            node->vars.push_back(variable);
        }

        return (ASTVarDeclStmt *) node;
    };

    virtual antlrcpp::Any visitVar_assgn(miniCParser::Var_assgnContext *context){
        // cout << "Visting var_assgn" << endl;
        ASTVarAssgnStmt *node = new ASTVarAssgnStmt();
        node->lhs = visit(context->var());
        if(context->expr() != nullptr){
            node->rhs = visit(context->expr());
        }
        else{
            ASTVarAssgnStmt *recurRhs = visit(context->var_assgn());
            node->rhs = recurRhs->rhs;
        }
        return (ASTVarAssgnStmt *)node;
    };

    virtual antlrcpp::Any visitVar2ID(miniCParser::Var2IDContext *context){
    	// cout << "Visiting var2id" << endl;
    	ASTVar *node = new ASTVar(0, nullptr, nullptr, 
    					context->ID()->getText(), "undefined");
    	return (ASTVar *) node;
    };

    virtual antlrcpp::Any visitVar2IDexpr(miniCParser::Var2IDexprContext *context){
    	// cout << "Visiting var2idexpr" << endl;
    	ASTVar *node = new ASTVar(1, visit(context->expr()), nullptr, 
    					context->ID()->getText(), "undefined");
    	return (ASTVar *) node;
    };

    virtual antlrcpp::Any visitVar2IDexprexpr(miniCParser::Var2IDexprexprContext *context){
    	// cout << "Visiting var2idexprexpr" << endl;
    	ASTVar *node = new ASTVar(2, visit(context->expr()[0]), 
    					visit(context->expr()[1]), 
    					context->ID()->getText(), "undefined");
    	return (ASTVar *) node;
    };

    virtual antlrcpp::Any visitExpr2value(miniCParser::Expr2valueContext *context){
        // cout << "Visiting expr2value" << endl;
        ASTValue *val = visit(context->value());
        return (ASTExpr *) val;
    };

    virtual antlrcpp::Any visitExpr2un_op(miniCParser::Expr2un_opContext *context){
        // cout << "Visiting expr2un_op" << endl;
        ASTUnaryExpr *node = new ASTUnaryExpr();
        node->un_op = context->un_op()->getText();
        node->expr = visit(context->expr());
        return (ASTExpr *) node;
    };

    virtual antlrcpp::Any visitExpr2var(miniCParser::Expr2varContext *context){
    	// cout << "Visiting expr2var" << endl;
    	ASTVar *var = visit(context->var());
    	return  (ASTExpr *) var;
    };

    virtual antlrcpp::Any visitExpr2bin_op(miniCParser::Expr2bin_opContext *context){
        // cout << "Visiting expr2bin_op" << endl;
        ASTBinaryExpr *node = new ASTBinaryExpr();
        node->left = visit(context->expr()[0]);
        node->right = visit(context->expr()[1]);
        if(context->a_op1() != nullptr)
            node->op = context->a_op1()->getText();
        else if(context->a_op2() != nullptr)
            node->op = context->a_op2()->getText();
        else if(context->r_op1() != nullptr)
            node->op = context->r_op1()->getText();
        else if(context->r_op2() != nullptr)
            node->op = context->r_op2()->getText();
        else if(context->r_op3() != nullptr)
            node->op = context->r_op3()->getText();
        else if(context->b_op1() != nullptr)
            node->op = context->b_op1()->getText();
        else if(context->b_op2() != nullptr)
            node->op = context->b_op2()->getText();
        else if(context->l_op1() != nullptr)
            node->op = context->l_op1()->getText();
        else if(context->l_op2() != nullptr)
            node->op = context->l_op2()->getText();

        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExpr2paran(miniCParser::Expr2paranContext *context){
        // cout << "Visiting expr2paran" << endl;
        ASTExpr *node = visit(context->expr());
        return (ASTExpr *) node;
    };

    virtual antlrcpp::Any visitExpr2condop(miniCParser::Expr2condopContext *context){
        // cout << "Visiting expr2condop" << endl;
        ASTCondExpr *node = new ASTCondExpr();
        node->cond = visit(context->expr()[0]);
        node->left = visit(context->expr()[1]);
        node->right = visit(context->expr()[2]);
        return (ASTExpr *) node;
    };

    virtual antlrcpp::Any visitExpr2func_call(miniCParser::Expr2func_callContext *context){
        // cout << "Visitin expr2funccall" << endl;
        ASTFuncCallStmt *node = visit(context->func_call());
        return (ASTExpr *) node;
    };

    virtual antlrcpp::Any visitValue(miniCParser::ValueContext *context){
        // cout << "Visiting value" << endl;
        ASTValue *node = new ASTValue();
        if(context->INT() != nullptr){
            node->type = "int";
            node->val = context->INT()->getText();
        }
        else if(context->UINT() != nullptr){
            node->type = "uint";
            node->val = context->UINT()->getText();    
        }
        else if(context->CHAR() != nullptr){
            node->type = "char";
            node->val = context->CHAR()->getText();    
        }
        else if(context->BOOL() != nullptr){
            node->type = "bool";
            node->val = context->BOOL()->getText();    
        }
        else if(context->STR() != nullptr){
            node->type = "string";
            node->val = context->STR()->getText();    
        }
        return (ASTValue *) node;
    };

    virtual antlrcpp::Any visitType(miniCParser::TypeContext *context){
        return NULL;
    };

    virtual antlrcpp::Any visitUn_op(miniCParser::Un_opContext *context){
        return NULL;
    };

    virtual antlrcpp::Any visitA_op1(miniCParser::A_op1Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitA_op2(miniCParser::A_op2Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitR_op1(miniCParser::R_op1Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitR_op2(miniCParser::R_op2Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitR_op3(miniCParser::R_op3Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitB_op1(miniCParser::B_op1Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitB_op2(miniCParser::B_op2Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitL_op1(miniCParser::L_op1Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitL_op2(miniCParser::L_op2Context *context){
        return NULL;
    };

    virtual antlrcpp::Any visitAssgn_op(miniCParser::Assgn_opContext *context){
        return NULL;
    };
};