#ifndef TypeChecking_H
#define TypeChecking_H
#include "SyntaxTree.h"
#include "SymbolTable.h"
void TypeChecking(TreeNode *root);
void evalType(TreeNode *p, char *path);
int SimpleType(TreeNode *p);
int is_integer(TreeNode *p);
#endif
