ANTLR4=java -Xmx500M -cp "/usr/local/lib/antlr-4.8-complete.jar:$CLASSPATH" org.antlr.v4.Tool

antlr:	miniC.g4
		$(ANTLR4) -Dlanguage=Cpp -no-listener -visitor miniC.g4 
		clang++-11 -g main.cpp `llvm-config-11 --cxxflags --ldflags --system-libs --libs core` -o miniC -I/runtime/Cpp/run/usr/local/include/antlr4-runtime -lantlr4-runtime -fexceptions
# 		g++ -g main.cpp -o miniC -I/runtime/Cpp/run/usr/local/include/antlr4-runtime -lantlr4-runtime
clean:	
		rm -f miniCLexer.h miniCParser.h miniC *.tokens *.interp miniCBaseVisitor.cpp \
		rm -f miniCBaseVisitor.h miniCParser.cpp miniCLexer.cpp miniCBaseVisitor.cpp  \
		rm -f miniCVisitor.cpp  miniCVisitor.h