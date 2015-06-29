#ifndef SymbolTable_H
#define SymbolTable_H
#include "SyntaxTree.h"
/* TypeList element declaration */
typedef struct Symbol{
	struct Symbol *next;
	char name[256];
	TreeNode *content;
} Symbol;
/* SymbolTable declaration*/
typedef struct SymbolTable{
	struct SymbolTable *sibling;
	Symbol *NameList[256];
	int nestLevel;
	char path[256];
	// int size;
} SymbolTable;

/* function declaration*/
int hash(char *key);
SymbolTable *CreateSymbolTable(char *path, int nestLevel);
void InsertSymbol(char *name, TreeNode *node, SymbolTable *symtab);
void Insert(char *name, char *path, TreeNode *node);
int sub_str(char *src, char *dst);
TreeNode *LookupSymbol(char *name, SymbolTable *symtab);
TreeNode *Lookup(char *name, char *path);
int ExistSymbolTable(char *path);
void ConnectSymtabs(SymbolTable *p1, SymbolTable *p2);
void SymtabTraverseTree(TreeNode *p, char *path, int nestLevel, SymbolTable *symtab);
#endif
