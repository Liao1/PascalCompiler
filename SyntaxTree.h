#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H
#include <iostream>
using namespace std;
/* Node type define*/
typedef enum { 
	/* top */
	routine_kind,
	/* const part */
	const_kind,	
	/* type part */
	type_kind, 
	/* var part */
	var_kind, 
	/* routine part */
	sub_kind,
	/* routine body */
	stmt_kind, 
	expr_kind
} NodeKind;
/* type_kind */
typedef enum {
	decl_type,
	integer_type,
	real_type,
	boolean_type,
	char_type,
	string_type,
	enum_type,
	array_type,
	record_type,
	//record_domain_type,
	selfdefined_type
} TypeKind;
/* sub_kind*/
typedef enum {
	func_kind,
	proc_kind,
	param_var_kind,
	param_val_kind
} SubKind;
/* stmt_kind */
typedef enum {
	assign_stmt,
	proc_stmt, 
	if_stmt, 
	repeat_stmt, 
	while_stmt, 
	for_stmt, 
	case_stmt, 
	goto_stmt,
	case_exp_stmt
} StmtKind;
/* expr_kind */
typedef enum {
	op_kind,	// operation
	id_kind,	// identifer
	fn_kind,	// function_kind
	con_kind	// const value kind
} ExprKind;
/* id_kind */
typedef enum { Basic, Array, Record } IdKind;
/* op_kind */
typedef enum {
	plus_kind,	// plus
	minus_kind,	// minus
	or_kind,	// or
	mul_kind,	// muliply
	div_kind,	// div
	mod_kind,	// mod
	and_kind,	// and
	ge_kind,	// greater equal
	gt_kind,	// greater than
	le_kind,	// less equal
	lt_kind,	// less than
	eq_kind,	// equal
	ueq_kind,	// unequal
	not_kind,	// not
	neg_kind	// negative
} OpKind;
/* attr */
typedef struct {
	int intVal;
	float realVal;
	char strVal[256];
} Attr;
/* Tree Node*/
#define MaxChildren 4
typedef struct TreeNode{
	struct TreeNode * children[MaxChildren], *sibling;
	int lineNo;
	/* universal */	
	NodeKind node;
	/* sub_kind */
	SubKind sub;
	/* type_kind */
	TypeKind type;
	/* stmt_kind */
	StmtKind stmt;
	/* expr_kind */
	ExprKind expr;
	/* id_kind */
	IdKind id;
	/* op_kind */
	OpKind op;
	/* attr */
	Attr attr;
	/* name */
	char name[256];
	/* label */
	int label;
	/* system function/procedure */
	int system;
	struct TreeNode *dtype;
} TreeNode;
/* function define */
/* global */
TreeNode *CreateNode(NodeKind kind);
/* routine */
TreeNode *CreateRoutine(TreeNode* routine_head, TreeNode* routine_body);
/* const part */
TreeNode *CreateConst(char* ConstName, TreeNode* ConstValue);
/* type part */
TreeNode *CreateSimpleType(TypeKind type);
TreeNode *CreateEnumType(TreeNode *exp1, TreeNode *exp2);
TreeNode *CreateArrayType(TreeNode *enumT, TreeNode *baseT);
TreeNode *CreateRecordType(TreeNode *domain);
//TreeNode *CreateDomainType(TreeNode *namelist, TreeNode *type);
TreeNode *CreateSelfDefineType(char *name);
TreeNode *CreateTypeDef(char *TypeName, TreeNode *type);
/* var part */
TreeNode *CreateVar(TreeNode* VarNameList, TreeNode* TypeDecl);
/* expr part*/
TreeNode *CreateConstExp(TypeKind type);
TreeNode *CreateIdExp(char *name);
TreeNode *CreateIdArrayExp(char *name, TreeNode *exp);
TreeNode *CreateIdRecordExp(char *name, char *domain);
TreeNode *CreateOpExp(OpKind op);
TreeNode *CreateFuncExp(char *name, TreeNode *args);
/* stmt part */
TreeNode *CreateStmtNode(StmtKind stmt);
/* function/procedure part */
TreeNode *CreateFunction(TreeNode *func_head, TreeNode *sub_routine);
TreeNode *CreateFuncHead(char *name, TreeNode *param, TreeNode *type);
TreeNode *CreateProcedure(TreeNode *proc_head, TreeNode *sub_routine);
TreeNode *CreateProcHead(char *name, TreeNode *param);
TreeNode *CreateParam(TreeNode *list, TreeNode *type, SubKind k);
/*================================================*/
void strCatch(char *d, char *s);
TreeNode *makeLabel(TreeNode *p, int label);
TreeNode *ConnectNodes(TreeNode *p, TreeNode*q);
/*================================================*/
void print_expr_op(TreeNode *p);
void print_expr_id(TreeNode *p);
void print_expr_fn(TreeNode *p);
void print_expr_con(TreeNode *p);
void print_expr(TreeNode *p);
void print_stmt(TreeNode *p);
void print_sub(TreeNode *p);
void print_tree(TreeNode *p, int level);
extern int lineNo;
extern FILE *fout;
extern TreeNode *root;
//extern SymbolTable *SymTab;
#endif
