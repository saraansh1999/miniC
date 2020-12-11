#include<string>
#include<set>
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
using namespace llvm;

class symbolTableNode;
class symbolTable;

typedef struct row{
	string name, type;
	int dim;
	Value *r, *c;
	AllocaInst* addr;

	row(){
	}

	row(string n, string t, int d, AllocaInst* a = NULL, Value* rows = NULL, Value* cols = NULL){
		name = n;
		type = t;
		dim = d;
		addr = a;
		r = rows;
		c = cols;
	}
	bool operator<(const row& rhs) const{
		return name < rhs.name;
	}
	bool operator==(const row& rhs) const{
		return name == rhs.name;
	}

	string getType() const{
		return type;
	}

}row;

typedef struct func{
	string name, type;
	vector<row> params;
	AllocaInst* addr;

	func(string n, string t, vector<row> ps, AllocaInst* a = NULL){
		name = n;
		type = t;
		params = ps;
		addr = a;
	}
	bool operator<(const func& rhs) const{
		return name < rhs.name;
	}
	bool operator==(const func& rhs) const{
		return name == rhs.name;
	}
	vector<string> getTypes() const{
		vector<string> typs;
		for(auto p: this->params){
			typs.push_back(p.getType());
		}
		return typs;
	}
}func;


class symbolTableNode{

public:
	
	string scope;
	std::set <row> rows;
	std::set <func> funcs;
	symbolTableNode* par;

	symbolTableNode(string sc, symbolTableNode* parent=NULL){
		this->scope = sc;
		this->par = parent;
	}

	const func* checkFuncExists(string name){
		set<func>::iterator it = this->funcs.find(func(name, "", vector<row>(0)));
		if(it == this->funcs.end())
			return NULL;
		return &(*it);
	}

	const row* checkVarExists(string name){
		set<row>::iterator it = this->rows.find(row(name, "", 0));
		if(it == this->rows.end())
			return NULL;
		return &(*it);
	}

	bool checkExists(string name){
		return (checkFuncExists(name) != NULL || 
				checkVarExists(name) != NULL);
	}

	bool addRow(string name, string type, int dim, AllocaInst *a, Value *r, Value *c){
		if(this->checkExists(name)){
			error("ERROR: Variable redeclaration for " + name);
			return 0;
		}

		this->rows.insert(row(name, type, dim, a, r, c));
		return 1;
	}

	bool addFunc(string name, string type, vector<row> params, AllocaInst *a){
		if(this->checkExists(name)){
			error("ERROR: Variable redeclaration for " + name);
			return 0;
		}
		this->funcs.insert(func(name, type, params, a));
		return 1;
	}
};

class symbolTable{
private:
	symbolTableNode *root, *cur;
public:
	symbolTable(){
		root = NULL;
		cur = root;
	}

	void addNewScope(string sc = ""){
		symbolTableNode *node = new symbolTableNode(sc, cur);
		cur = node;
		// std::cout << "------------" << cur->scope << "--------" << std::endl;
	}

	void exitScope(){
		symbolTableNode *node = cur;
		cur = cur->par;
		delete(node);
		// if(cur == NULL)
		// 	std::cout << "------------end--------" << std::endl;	
		// else
		// 	std::cout << "------------" << cur->scope << "--------" << std::endl;
	}

	const row* findVar(string name){
		symbolTableNode *node = cur;
		const row* it;
		while(node != NULL){
			it = node->checkVarExists(name); 
			if(it != NULL){
				return &(*it);
			}
			else{
				node = node->par;
			}
		}
		error("ERROR: Variable " + name + " undeclared");
		return NULL;
	}

	const func* findFunc(string name){
		symbolTableNode *node = cur;
		const func* it;
		while(node != NULL){
			it = node->checkFuncExists(name);
			if(it != NULL){
				return &(*it);
			}
			else{
				node = node->par;
			}
		}
		error("ERROR: Variable " + name + " undeclared");
		return NULL;
	}

	bool isDigit(char a){
		return (a >= '0' && a <= '9');
	}

	bool isLoop(string str){
		return (str == ".loop");
	}

	string findEnclosingFunc(){
		symbolTableNode *node = cur;
		while(node != NULL){
			if(isDigit(node->scope[0]) || isLoop(node->scope))
				node = node->par;
			else{
				string id = node->scope;
				node = node->par;
				return node->checkFuncExists(id)->type;
			}
		}
		error("ERROR: Return statement should be within a function.");
		return "";
	}

	string getCurScope(){
		return cur->scope;
	}

	bool findEnclosingLoop(){
		symbolTableNode *node = cur;
		while(node != NULL){
			if(isDigit(node->scope[0]))
				node = node->par;
			else if(!isLoop(node->scope)){
				break;
			}
			else{
				return 1;
			}
		}
		error("ERROR: Break/Continue should be within a loop.");
		return 0;	
	}

	bool addNewVariable(string name, string type, int dim, AllocaInst *a=NULL, Value *r = NULL, Value *c = NULL){
		return cur->addRow(name, type, dim, a, r, c);
	}

	bool addNewFunction(string name, string type, vector<row> params, AllocaInst *a=NULL){
		return cur->addFunc(name, type, params, a);
	}

};