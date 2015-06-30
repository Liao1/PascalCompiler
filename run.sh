#!/bin/sh
bison compiler.y
flex compiler.l
g++ SymbolTable.c SyntaxTree.c TypeChecking.c MiddleWare.cpp node.cpp AST.cpp main.cpp -o test -Iinclude -I. `llvm-config --cppflags --libs core jit native --ldflags all-targets asmparser` -ltinfo -ldl -lpthread -std=c++11
./test test.in test.out

