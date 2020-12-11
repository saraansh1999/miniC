#include <iostream>
using namespace std;
#include <string>
#include "utils.cpp"

#include "antlr4-runtime.h"
#include "miniCLexer.cpp"
#include "miniCParser.cpp"
// #include "ast.h"

#include "miniCBuildASTVisitor.h"
#include "semanticVisitor.h"
#include "llvmIRGeneratorVisitor.h"

// #include "PostfixVisitor.h"

using namespace antlr4;

int main(int argc, const char* argv[]) {
    std::ifstream stream;

    cout << "Processing input file " << argv[1] << endl;
    stream.open(argv[1]);
    
    ANTLRInputStream input(stream);

    miniCLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    miniCParser parser(&tokens);    

    miniCParser::ProgContext *ctx = parser.prog();

    cout << "-------------------------------------\n";

    cout << "Constructing AST.....\n";

    miniCVisitor *visitor = new miniCBuildASTVisitor();
    ASTProg *program_root = visitor->visitProg(ctx);

    cout << "AST constructed.\n";

    cout << "-------------------------------------\n";

    cout << "Performing Semantic Analysis......\n";

    semanticVisitor *semanticAnalyser = new semanticVisitor();
    semanticAnalyser->visit(program_root);

    cout << "Semantic Analysis Completed\n";

    cout << "-------------------------------------\n";

    cout << "Performing Code Generation...\n";

    llvmIRGeneratorVisitor *irGenerator = new llvmIRGeneratorVisitor();
    irGenerator->visit(program_root);
    
    cout << "Code Generation Completed\n";

    cout << "-------------------------------------\n\n\n\n";

    string IR;
    raw_string_ostream OS(IR);
    OS << TheModule;
    OS.flush();
    ofstream writer("ir_llvm.ll");
    writer << IR;
    writer.close();
    cout << "LLVM IR written to ir_llvm.ll" << endl;
    // cout << IR;
    // TheModule.print(errs(), nullptr);

    return 0;
}
