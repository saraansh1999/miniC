#ifndef AST_H
#include "ast.h"
#define AST_H
#endif
#ifndef SYMBOL_TABLE_H
#include "symbolTable.h"
#define SYMBOL_TABLE_H
#endif
#include<stdlib.h>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <stack>

#define pb push_back

using namespace llvm;

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static Module TheModule("myModule", TheContext);
stack<BasicBlock*> loopCondBlocks, loopMergeBlocks;
string globalScope = "|global|";

string getFormat(Type* typ){
	// change this to include unsigned, strings
	if(typ == Type::getInt32Ty(TheContext)){
		return "%d";
	}	
	else if(typ == Type::getInt1Ty(TheContext)){
		// change this
		return "%d";
	}
	else if(typ == Type::getInt8Ty(TheContext)){
		return "%c";
	}

	return "";
}

Type* getLLVMType(string typ){
	// change/check for negative numbers
	if(isInt(typ)){
		return Type::getInt32Ty(TheContext);
	}
	else if(isUint(typ)){
		return Type::getInt32Ty(TheContext);
	}
	else if(isBool(typ)){
		return Type::getInt1Ty(TheContext);
	}
	else{
		return Type::getInt8Ty(TheContext);
	}
}

char parseChar(string s){
	if(s[1] != '\\'){
		switch(s[1]){
			case '\\':  error("ERROR: Invalid escape character"); exit(0);
			case '\'':  error("ERROR: Invalid escape character"); exit(0);
		}
		return s[1];
	}
	if(s.size() != 4){
		error("ERROR: Invalid escape character"); exit(0);
	}
	switch(s[2]){
		case 'n': return '\n';
		case '\\': return '\\';
		case '\'': return '\'';
		default:  error("ERROR: Invalid escape character"); exit(0);
	}
}

Value* getLLVMValue(string val, string typ){
	// change this to include arrays and strings
	if(isInt(typ)){
		return ConstantInt::get(getLLVMType(typ), stoul(val), true);
	}
	else if(isUint(typ)){
		return ConstantInt::get(getLLVMType(typ), stoul(val), false);
	}
	else if(isBool(typ)){
		if(val == "true")
			return ConstantInt::getTrue(TheContext);
		else
			return ConstantInt::getFalse(TheContext);
	}
	else if(isChar(typ)){
		// change this to handle more escape sequences
		return ConstantInt::get(getLLVMType(typ), (int)(parseChar(val)), false);
	}
	else{
		// string
		vector<Constant*> chars;
		Type *T = getLLVMType("char");
		for(auto c: val)
			chars.pb(ConstantInt::get(T, (int)(parseChar(string(1, c))), false));
		ArrayType *AT = ArrayType::get(T, val.size());
		Constant* str = ConstantArray::get(AT, chars);
		return str;
	}
	return NULL;
}

static AllocaInst *createGlob(const ASTVar* var, Value* num = NULL) {
	Type *T = getLLVMType(var->type);
	GlobalVariable* glob;
	if(num == NULL)
		glob = new GlobalVariable(
			TheModule,
			T,
			false,
			GlobalValue::CommonLinkage,
			ConstantInt::get(getLLVMType(var->type), 0),
			var->name
		);
	else{
		ConstantInt* size = (ConstantInt*)num;
		ArrayType *AT = ArrayType::get(T, size->getZExtValue());
		glob = new GlobalVariable(
			TheModule,
			AT,
			false,
			GlobalValue::CommonLinkage,
			ConstantAggregateZero::get(AT),
			var->name
		);
	}
  return (AllocaInst *)glob;
}

static AllocaInst *allocateOnStack(const ASTVar *var, Value* num = NULL) {
  // change this to include dims
	Type *T = getLLVMType(var->type);
	if(num == NULL)
		return Builder.CreateAlloca(T, nullptr, var->name);
	else{
		ConstantInt* size = (ConstantInt*)num;
		ArrayType *AT = ArrayType::get(T, size->getZExtValue());
		return Builder.CreateAlloca(AT, nullptr, var->name);
	}
}

// change stuff for unsigned stuff

class llvmIRGeneratorVisitor : public ASTvisitor {
private:
	bool createInBlock;
	symbolTable symbols;
public:
	llvmIRGeneratorVisitor(){
		this->createInBlock = 1;

		// create IO functions
		FunctionType *FT = FunctionType::get(Builder.getInt32Ty(), {Type::getInt8PtrTy(TheContext)}, true);
		Function *scanF = Function::Create(FT, Function::ExternalLinkage, "scanf", TheModule);
		Function *printF = Function::Create(FT, Function::ExternalLinkage, "printf", TheModule);
	}

	virtual antlrcpp::Any visit(ASTNode *node){
		return NULL;
	}
    virtual antlrcpp::Any visit(ASTProg *node){
    	// cout<< "prog\n";
    	// dummy function for global space
    	FunctionType *FT = FunctionType::get(Type::getVoidTy(TheContext), vector<Type*>(), false);
    	Function *F = Function::Create(FT, Function::ExternalLinkage, "main", TheModule);
    	BasicBlock *bb = BasicBlock::Create(TheContext, "entry", F);
		Builder.SetInsertPoint(bb);
		this->createInBlock = 0;
		this->symbols.addNewScope(globalScope);
    	node->mainBlock->accept(this);
    	Builder.CreateRetVoid();
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTStmt *node){
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTBlock *node){
    	// cout<< "block\n";
    	if(this->createInBlock){
    		this->symbols.addNewScope();
    	}
    	this->createInBlock = 1;
    	
    	for(auto stmt: node->stmtList){
    		stmt->accept(this);
    	}
    	this->symbols.exitScope();
    	return (Value *)NULL;
    }
	virtual antlrcpp::Any visit(ASTExpr *node){
		// cout<< "expr\n";
		return NULL;
	}
    virtual antlrcpp::Any visit(ASTValue *node){
    	// cout<< "val\n";
    	// cout<< node->val << " " << node->type << endl;
    	return (Value *)getLLVMValue(node->val, node->type);
    }
    virtual antlrcpp::Any visit(ASTVar *node){
    	// cout<< "var\n";
    	AllocaInst* addr = node->acceptForLHS(this);
	    return (Value *)Builder.CreateLoad(addr, "load");
    }
    virtual antlrcpp::Any visitForLHS(ASTVar *node){
    	// cout<< "varLHS\n";
    	const row* var = this->symbols.findVar(node->name);
    	Value *addr = (Value *)var->addr;
    	Value *zero = getLLVMValue("0", "int");
    	if(node->dim == 1){
    		Value *num = (node->r->accept(this));
    		addr = Builder.CreateGEP(
    				addr, 
    				{zero, num},
    				"gep"
    			);
	    	// addr = Builder.CreateAdd(addr, num, "index_add");
	    }
	    else if(node->dim == 2){
	    	Value *num0 = node->r->accept(this);
	    	Value *num1 = node->c->accept(this);
	    	addr = Builder.CreateGEP(
	    				addr, 
	    				{zero, Builder.CreateAdd(
				    		num1,
				    		Builder.CreateMul(
				    			num0, var->c, "index_mul"
				    		), "index_add"),
	    				}, "gep");
	    }
    	return (AllocaInst *)(addr);
    }
    virtual antlrcpp::Any visitToAlloc(ASTVar *node, bool global=false){
    	// cout<< "varAlloc\n";
    	AllocaInst *Alloca;
    	Value *num = NULL, *num0 = NULL, *num1 = NULL;
    	if(node->dim == 1){
	    	num0 = node->r->accept(this);
	    	num = num0;
	    }
	    else if(node->dim == 2){
	    	num0 = node->r->accept(this);
	    	num1 = node->c->accept(this);
	    	num = Builder.CreateMul(num0, num1, "dim_mul");
	    }

	    if(global)
	    	Alloca = Alloca = createGlob(node, num);
	    else
	    	Alloca = allocateOnStack(node, num);

    	this->symbols.addNewVariable(node->name, node->type, node->dim, Alloca, num0, num1);
		return (AllocaInst *)Alloca;
    }
    virtual antlrcpp::Any visit(ASTVarDeclStmt *node){
    	// cout<< "vardecl\n";
    	for(auto var: node->vars){
    		AllocaInst *Alloca;
    		if(this->symbols.getCurScope() == globalScope){
    			Alloca = var->acceptToAlloc(this, true);
    		}
    		else
    			Alloca = var->acceptToAlloc(this);
    	}
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTVarAssgnStmt *node){
    	// change this to add support for a = b = 1;
    	// cout<< "varassgn\n";
    	AllocaInst *addr = node->lhs->acceptForLHS(this);    	
    	Value* val = node->rhs->accept(this);
    	Builder.CreateStore(val, (AllocaInst *)addr, "store");
    	return val;
    }
    virtual antlrcpp::Any visit(ASTInputStmt *node){
    	Function *F = TheModule.getFunction("scanf");
    	AllocaInst *Alloca;
    	Value *format;
    	for(auto var: node->vars){
    		Alloca = var->acceptForLHS(this); 
    		format = Builder.CreateGlobalStringPtr(getFormat(Alloca->getType()->getElementType()));
    		Builder.CreateCall(F, {format, Alloca}, "funccall");
    	}
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTOutputStmt *node){
    	Function *F = TheModule.getFunction("printf");
    	Value* val;
    	Value *format;
    	for(auto out: node->outs){
    		val = out->accept(this);
    		format = Builder.CreateGlobalStringPtr(getFormat(val->getType()));
    		Builder.CreateCall(F, {format, val}, "funccall");
    	}
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTUnaryExpr *node){
    	// cout<< "unexp\n";
    	Value* retval;
    	if(node->un_op == "-"){
    		Value *lhs = (Value *)ConstantInt::get(Type::getInt32Ty(TheContext), -1, true);
    		Value *rhs = node->expr->accept(this);
    		retval = Builder.CreateMul(lhs, rhs, "unary_negation");
    	}
    	else{
    		retval = Builder.CreateNot(node->expr->accept(this), "unary_not");
    	}
    	return retval;
    }
    virtual antlrcpp::Any visit(ASTBinaryExpr *node){
    	// cout<< "binexp\n";
    	Value *retval;
    	Value *left= node->left->accept(this);
    	Value *right = node->right->accept(this);
    	if(node->op == "+")
    		retval = Builder.CreateAdd(left, right, "bin_add"); 
    	else if(node->op == "-")
    		retval = Builder.CreateSub(left, right, "bin_sub");
    	else if(node->op == "*")
    		retval = Builder.CreateMul(left, right, "bin_mul");
    	else if(node->op == "/")
    		retval = Builder.CreateSDiv(left, right, "bin_sdiv");
    	else if(node->op == "%")
    		retval = Builder.CreateSRem(left, right, "bin_smods");
    	else if(node->op == "&" || node->op == "&&")
    		retval = Builder.CreateAnd(left, right, "bin_and");
    	else if(node->op == "|" || node->op == "||")
    		retval = Builder.CreateOr(left, right, "bin_or");
    	else if(node->op == "==")
    		retval = Builder.CreateICmpEQ(left, right, "bin_eq");
    	else if(node->op == "!=")
    		retval = Builder.CreateICmpNE(left, right, "bin_neq");
    	else if(node->op == ">")
    		retval = Builder.CreateICmpSGT(left, right, "bin_sgt");
    	else if(node->op == ">=")
    		retval = Builder.CreateICmpSGE(left, right, "bin_sge");
    	else if(node->op == "<")
    		retval = Builder.CreateICmpSLT(left, right, "bin_slt");
    	else if(node->op == "<=")
    		retval = Builder.CreateICmpSLE(left, right, "bin_sle");
    	return retval;
    }
    virtual antlrcpp::Any visit(ASTCondExpr *node){
    	// cout<< "condexpr\n";
   		Function *F = Builder.GetInsertBlock()->getParent();

    	Value* cond = node->cond->accept(this);

    	BasicBlock *trueBlock = BasicBlock::Create(TheContext, "ternaryTrueBlock");
    	BasicBlock *falseBlock = BasicBlock::Create(TheContext, "ternaryFalseBlock");
    	BasicBlock *mergeBlock = BasicBlock::Create(TheContext, "ternaryMergeBlock");

    	Builder.CreateCondBr(cond, trueBlock, falseBlock);

    	F->getBasicBlockList().pb(trueBlock);
    	Builder.SetInsertPoint(trueBlock);
    	Value *trueVal = node->left->accept(this);
    	Builder.CreateBr(mergeBlock);
    	trueBlock = Builder.GetInsertBlock();

  		F->getBasicBlockList().pb(falseBlock);
    	Builder.SetInsertPoint(falseBlock);
    	Value *falseVal = node->right->accept(this);
    	Builder.CreateBr(mergeBlock);
    	falseBlock = Builder.GetInsertBlock();
  	
    	F->getBasicBlockList().pb(mergeBlock);
    	Builder.SetInsertPoint(mergeBlock);
    	PHINode *phi = Builder.CreatePHI(trueVal->getType(), 2, "ternaryPhi");
    	phi->addIncoming(trueVal, trueBlock);
    	phi->addIncoming(falseVal, falseBlock);

    	return (Value *)phi;
    }
    virtual antlrcpp::Any visit(ASTIfelseStmt *node){
    	// cout<< "ifelse\n";

    	Function *F = Builder.GetInsertBlock()->getParent();
    	
    	BasicBlock *mergeBlock = BasicBlock::Create(TheContext, "mergeBlock");

    	int i;
    	for(i = 0; i < node->conds.size(); i++){
    		
    		Value *cond = node->conds[i]->accept(this);
			
			BasicBlock *thisBlock = BasicBlock::Create(TheContext, "ifBlock");
    		BasicBlock *nextBlock;
    		
    		if(i == node->conds.size() - 1){
    			// if{}....elif{} pattern
    			if(i == node->blocks.size() - 1){
    				nextBlock = mergeBlock;
    			}
    			// if{}....else{} pattern
    			else{
    				nextBlock = BasicBlock::Create(TheContext, "elseBlock");	
    			}
    		}
    		// if{}...elif{}.... pattern
    		else{
				nextBlock = BasicBlock::Create(TheContext, "elifCondBlock");
				
    		}
			
			Builder.CreateCondBr(cond, thisBlock, nextBlock);

			F->getBasicBlockList().pb(thisBlock);    			
			Builder.SetInsertPoint(thisBlock);
			node->blocks[i]->accept(this);
			Builder.CreateBr(mergeBlock);

			F->getBasicBlockList().pb(nextBlock);    			
			Builder.SetInsertPoint(nextBlock);
    	}
    	
    	// handling the last else{} block
    	if(i < node->blocks.size()){
			node->blocks[i]->accept(this);
			Builder.CreateBr(mergeBlock);    		

			F->getBasicBlockList().pb(mergeBlock);
	    	Builder.SetInsertPoint(mergeBlock);
    	}

    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTWhileStmt *node){
    	// cout<< "whileloop\n";
    	this->createInBlock = 0;
    	this->symbols.addNewScope(".loop");
    	
    	Function *F = Builder.GetInsertBlock()->getParent();

		BasicBlock *condBlock = BasicBlock::Create(TheContext, "whileCondBlock", F);
		BasicBlock *loopBlock = BasicBlock::Create(TheContext, "whileLoopBlock", F);
		BasicBlock *mergeBlock = BasicBlock::Create(TheContext, "whileMergeBlock", F);

		Builder.CreateBr(condBlock);

		Builder.SetInsertPoint(condBlock);
		Value *cond = node->cond->accept(this);
		Builder.CreateCondBr(cond, loopBlock, mergeBlock);

		loopCondBlocks.push(condBlock);
		loopMergeBlocks.push(mergeBlock);

		Builder.SetInsertPoint(loopBlock);
		node->block->accept(this);
		Builder.CreateBr(condBlock);

		loopCondBlocks.pop();
		loopMergeBlocks.pop();

		Builder.SetInsertPoint(mergeBlock);
 		
 		return NULL;
    }
    virtual antlrcpp::Any visit(ASTBreakStmt *node){
    	// cout<< "break\n";
    	Builder.CreateBr(loopMergeBlocks.top());
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTContinueStmt *node){
    	// cout<< "continue\n";
    	Builder.CreateBr(loopCondBlocks.top());
    	return NULL;
    }
    virtual antlrcpp::Any visit(ASTReturnStmt *node){
    	Value *retval = node->expr->accept(this);
    	return (Value *)Builder.CreateRet(retval);;
    }
    virtual antlrcpp::Any visit(ASTFuncDeclStmt *node){
    	// cout<< "funcdecl\n";

    	// remember the current BB so that we can go back to it
    	BasicBlock *curBlock = Builder.GetInsertBlock();

    	// create function in llvm
	    // change this to inclide arrays
	    vector<Type*> param_types;
	    for(auto p: node->params){
	    	param_types.pb(getLLVMType(p->type));
	    }
    	FunctionType *FT = FunctionType::get(getLLVMType(node->ret_type), param_types, false);
    	Function *F = Function::Create(FT, Function::ExternalLinkage, node->name, TheModule);
		////////////////////

		// create new basic block for function
    	BasicBlock *bb = BasicBlock::Create(TheContext, node->name, F);
		Builder.SetInsertPoint(bb);
		///////////////////////////////////

		// add in symbol table and allocate params on stack
		vector<row> ps;
    	for(auto p: node->params){
    		row temp(p->name, p->type, p->dim);
    		ps.push_back(temp);
    	}
    	this->symbols.addNewFunction(node->name, node->ret_type, ps);
  
    	this->createInBlock = 0;
    	this->symbols.addNewScope(node->name);

    	int i = 0;
    	for(auto &arg: F->args()){
    		AllocaInst* addr = node->params[i++]->acceptToAlloc(this);
    		// only primitive data types
    		Builder.CreateStore(&arg, addr, "store");
    	}
    	//////////////////////////////////////

    	// visit body of function
    	node->block->accept(this);
    	//////////////////////////////////////

    	Builder.CreateRet(Constant::getNullValue(getLLVMType(node->ret_type)));

    	verifyFunction(*F);

    	// revert to the previous BB
    	Builder.SetInsertPoint(curBlock);

		return F;
    }
    virtual antlrcpp::Any visit(ASTFuncCallStmt *node){

    	Function *F = TheModule.getFunction(node->name);
    	///////////
    	// change this to add array matching in params [type matching done in semantic checks]
    	////////
    	
    	vector<Value *> args;
    	for(auto p: node->params){
    		args.pb(p->accept(this));
    	}

    	return (Value *)Builder.CreateCall(F, args, "funcCall");
    }
    virtual antlrcpp::Any visit(ASTForStmt *node){
    	// cout<< "forloop\n";
    	this->createInBlock = 0;
    	this->symbols.addNewScope(".loop");

    	for(auto assgn: node->init)
			assgn->accept(this);

		Function *F = Builder.GetInsertBlock()->getParent();

		BasicBlock *condBlock = BasicBlock::Create(TheContext, "forCondBlock", F);
		BasicBlock *loopBlock = BasicBlock::Create(TheContext, "forLoopBlock", F);
		BasicBlock *mergeBlock = BasicBlock::Create(TheContext, "forMergeBlock", F);

		Builder.CreateBr(condBlock);

		Builder.SetInsertPoint(condBlock);
		Value *cond = node->cond->accept(this);
		Builder.CreateCondBr(cond, loopBlock, mergeBlock);

		loopCondBlocks.push(condBlock);
		loopMergeBlocks.push(mergeBlock);

		Builder.SetInsertPoint(loopBlock);
		node->block->accept(this);
		for(auto assgn: node->update)
			assgn->accept(this);
		Builder.CreateBr(condBlock);

		loopCondBlocks.pop();
		loopMergeBlocks.pop();

		Builder.SetInsertPoint(mergeBlock);

    	return NULL;
    }
};