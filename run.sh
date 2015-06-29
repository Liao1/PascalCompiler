!/bin/sh
bison compiler.y
flex compiler.l
gcc SymbolTable.c SyntaxTree.c TypeChecking.c lex.yy.c -o baibodiao
./baibodiao test.in test.out
cat test.out