#ifndef Global_H
#define Global_H
#include "SyntaxTree.h"
#include "SymbolTable.h"
/* global variable */
char path[256];
int nestLevel;
int lineNo;
FILE *fout;
TreeNode *root;
SymbolTable *SymTab;
int yylex(void);
void yyerror(char *);
#endif
