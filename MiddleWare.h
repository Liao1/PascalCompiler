#ifndef MiddleWare_H
#define MiddleWare_H
#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "node.h"
ConstAST *CreateConstAST(TreeNode *p);
BasicTypeAST *CreateTypeAST(TreeNode *p);
VariableDeclAST * CreateVariableAST(TreeNode *p);
// PrototypeAST * CreatePrototypeAST(TreeNode *p);
ExprAST *CreateExprAST(TreeNode *p, char *path);
vector<ExprAST *>CreateWriteAST(TreeNode *p, char *path);
vector<ExprAST *>CreateWriteLnAST(TreeNode *p, char *path);
ExprAST *CreateStmtExprAST(TreeNode *p, char *path);
FunctionAST *CreateFunctionAST(TreeNode *p, char *path);
#endif