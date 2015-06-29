#ifndef MiddleWare_H
#define MiddleWare_H
#include "SyntaxTree.h"
#include "node.h"
ConstAST *CreateConstAST(TreeNode *p);
TypeAST *CreateTypeAST(TreeNode *p);
VariableDeclAST * CreateVariableAST(TreeNode *p);
PrototypeAST * CreatePrototypeAST(TreeNode *p);
ExprAST *CreateExprAST(TreeNode *p);
ExprAST *CreateStmtExprAST(TreeNode *p);
FunctionAST *CreateFunctionAST(TreeNode *p);
#endif