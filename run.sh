!/bin/sh
bison compiler.y
flex compiler.l
g++ SymbolTable.c SyntaxTree.c TypeChecking.c lex.yy.c -o baibodiao -Iinclude -I. `llvm-config --cppflags --libs core jit native --ldflags all-targets asmparser` -ltinfo -ldl
./baibodiao test.in test.out
cat test.out