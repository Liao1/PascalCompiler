#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SyntaxTree.h"
#include "Global.h"
/* new tree node */
TreeNode *CreateNode(NodeKind kind){
	TreeNode *p = (TreeNode *)malloc(sizeof(TreeNode));
	p->children[0] = p->children[1] = p->children[2] = p->children[3] = p->sibling = 0;
	p->node = kind;
	p->label = 0;
	p->system = 0;
	p->lineNo = lineNo;
	p->dtype = NULL;
	return p;
}
/*=============================universal over=================================*/
/*=============================header=========================================*/
/*=============================routine=========================================*/
TreeNode *CreateRoutine(TreeNode* routine_head, TreeNode* routine_body){
	TreeNode *p = CreateNode(routine_kind);
	p->children[0] = routine_head;
	p->children[1] = routine_body;
	return p;
}
/*=============================const_part=========================================*/
/* const part */
TreeNode *CreateConst(char* ConstName, TreeNode* ConstValue){
	TreeNode *p = CreateNode(const_kind);
	p->children[0] = ConstValue;
	strcpy(p->name,ConstName); 
	return p;
}
/*=============================var_part=========================================*/
/* var part */
TreeNode *CreateVar(TreeNode* VarNameList, TreeNode* TypeDecl){
	TreeNode *p = CreateNode(var_kind);
	p->children[0] = VarNameList;
	p->dtype = TypeDecl;
	return p;
}
/*=============================type_part=========================================*/
/* type part */
TreeNode *CreateSimpleType(TypeKind type){
	TreeNode *p = CreateNode(type_kind);
	p->type = type;
	return p;
}
TreeNode *CreateEnumType(TreeNode *exp1, TreeNode *exp2){
	TreeNode *p = CreateNode(type_kind);
	p->children[0] = exp1; p->children[1] = exp2;
	p->type = enum_type;
	return p;
}
TreeNode *CreateArrayType(TreeNode *enumT, TreeNode *baseT){
	TreeNode *p = CreateNode(type_kind);
	p->children[0] = enumT; p->children[1] = baseT;
	p->type = array_type;
	return p;
}
TreeNode *CreateRecordType(TreeNode *domain){
	TreeNode *p = CreateNode(type_kind);
	p->children[0] = domain;
	p->type = record_type;
	return p;
}
/*
TreeNode *CreateDomainType(TreeNode *namelist, TreeNode *type){
	TreeNode *p = CreateNode(type_kind);
	p->children[0] = namelist; p->children[1] = type;
	p->type = record_domain_type;
	return p;
}
*/
TreeNode *CreateSelfDefineType(char *name){
	TreeNode *p = CreateNode(type_kind);
	p->type = selfdefined_type;
	strcpy(p->name, name);
	return p;
}
TreeNode *CreateTypeDef(char *TypeName, TreeNode *type){
	TreeNode *p = CreateNode(type_kind);
	p->type = decl_type;
	p->dtype = type;
	strcpy(p->name, TypeName);
	return p;
}
/*=============================routine_part=========================================*/
/* function part */
TreeNode *CreateFuncHead(char *name, TreeNode *param, TreeNode *type){
	TreeNode *p = CreateNode(sub_kind);
	p->children[0] = param; p->dtype = type;
	p->sub = func_kind;
	strcpy(p->name, name);
	return p;
}
/* procedure part */
TreeNode *CreateProcHead(char *name, TreeNode *param){
	TreeNode *p = CreateNode(sub_kind);
	p->children[0] = param;
	p->sub = proc_kind;
	strcpy(p->name, name);
	return p;
}
/* param part */
TreeNode *CreateParam(TreeNode *list, TreeNode *type, SubKind k){
	TreeNode *p = CreateNode(sub_kind);
	p->children[0] = list; p->dtype = type;
	p->sub = k;
	return p;
}
/*=============================header over====================================*/
/* Init a const exp node */
TreeNode *CreateConstExp(TypeKind type){
	TreeNode *p = CreateNode(expr_kind);
	p->dtype = CreateSimpleType(type);
	p->expr = con_kind;
	return p;
}
/* Init an identifier exp node */
TreeNode *CreateIdExp(char *name){
	TreeNode *p = CreateNode(expr_kind);
	p->expr = id_kind;
	p->id = basic;
	strcpy(p->name, name);
	return p;
}
TreeNode *CreateIdArrayExp(char *name, TreeNode *exp){
	TreeNode *p = CreateNode(expr_kind);
	p->children[0] = exp;
	p->expr = id_kind;
	p->id = array;
	strcpy(p->name, name);
	return p;
}
TreeNode *CreateIdRecordExp(char *name, char *domain){
	TreeNode *p = CreateNode(expr_kind);
	p->children[0] = CreateIdExp(domain);
	p->expr = id_kind;
	p->id = record;
	strcpy(p->name, name);
	return p;
}
/* Init an Operation exp node */
TreeNode *CreateOpExp(OpKind op){
	TreeNode *p = CreateNode(expr_kind);
	p->expr = op_kind;
	p->op = op;
	return p;
}
/* Init a function exp node */
TreeNode *CreateFuncExp(char *name, TreeNode *args){
	TreeNode *p = CreateNode(expr_kind);
	p->children[0] = args;
	p->expr = fn_kind;
	strcpy(p->name, name);
	return p;
}
/*===========================expr over==========================================*/
/* Init a stmt node */
TreeNode *CreateStmtNode(StmtKind stmt){
	TreeNode *p = CreateNode(stmt_kind);
	p->stmt = stmt;
	return p;
}
/*==========================stmt over===========================================*/
/* Connect nodes */
TreeNode *ConnectNodes(TreeNode *p, TreeNode*q){
	TreeNode * ret;
	if (!p) return q;
	ret = p;
	while (p->sibling!=NULL) p = p->sibling;
	p->sibling = q;
	return ret;
}
/* extract string content */
void strCatch(char *d, char *s){
	int i;
	for (i=1; i<strlen(s)-1; i++){
		d[i-1] = s[i];
	}
	d[strlen(s)-1] = 0;
}
TreeNode *makeLabel(TreeNode *p, int label){
	p->label = label;
	return p;
}
/* print nodes to debug */
void print_const(TreeNode *p){
	fprintf(fout, "const: %s :", p->name);
}
void print_type(TreeNode *p){
	switch (p->type){
		case decl_type: fprintf(fout, "type of %s is:", p->name); break;
		case integer_type: fprintf(fout, "type: integer"); break;
		case real_type: fprintf(fout, "type: real"); break;
		case boolean_type: fprintf(fout, "type: boolean"); break;
		case char_type: fprintf(fout, "type: char"); break;
		case string_type: fprintf(fout, "type: string"); break;
		case enum_type: {
			fprintf(fout, "type: enum:"); 
			if (!p->children[1]) fprintf(fout, "names:"); else fprintf(fout, "exprs");  
			break;
		}
		case array_type: fprintf(fout, "type: array"); break;
		case record_type: fprintf(fout, "type: record"); break;
		//case record_domain_type: fprintf(fout, "type: record_domain"); break;
		case selfdefined_type: 	fprintf(fout, "selfdefined: %s", p->name); break;
	}
}
void print_var(TreeNode *p){
	fprintf(fout, "var:");
}
void print_expr_op(TreeNode *p){
	fprintf(fout, "Op_expr:");
	switch (p->op){
		case plus_kind: fprintf(fout, "plus"); break;
		case minus_kind: fprintf(fout, "minus"); break;
		case or_kind: fprintf(fout, "or"); break;
		case mul_kind: fprintf(fout, "mul"); break;
		case div_kind: fprintf(fout, "div"); break;
		case mod_kind: fprintf(fout, "mod"); break;
		case and_kind: fprintf(fout, "and"); break;
		case ge_kind: fprintf(fout, "ge"); break;
		case gt_kind: fprintf(fout, "gt"); break;
		case le_kind: fprintf(fout, "le"); break;
		case lt_kind: fprintf(fout, "lt"); break;
		case eq_kind: fprintf(fout, "eq"); break;
		case ueq_kind: fprintf(fout, "ueq"); break;
		case not_kind: fprintf(fout, "not"); break;
		case neg_kind: fprintf(fout, "neg"); break;
	}
}
void print_expr_id(TreeNode *p){
	switch (p->id){
		case basic: fprintf(fout, "Id_expr_basic: %s", p->name); break;
		case array: fprintf(fout, "Id_expr_array: %s", p->name); break;
		case record: fprintf(fout, "Id_expr_record: %s", p->name); break;
	}
}
void print_expr_fn(TreeNode *p){
	fprintf(fout, "Fn_expr: %s, args:", p->name);
}
void print_expr_con(TreeNode *p){
	fprintf(fout, "const_expr: ");
}
void print_expr(TreeNode *p){
	switch (p->expr){
		case op_kind: { print_expr_op(p); } break;
		case id_kind: { print_expr_id(p); } break;
		case fn_kind: { print_expr_fn(p); } break;
		case con_kind: { print_expr_con(p); } break;
	}
}
void print_stmt(TreeNode *p){
	if (p->label) fprintf(fout, "label:%d;", p->label);
	switch (p->stmt){
		case if_stmt: { fprintf(fout, "if_stmt:"); } break;
		case for_stmt: { fprintf(fout, "for:%s", p->name); if (p->attr.intVal) fprintf(fout, " to: "); else fprintf(fout, "downto"); } break;
		case repeat_stmt: { fprintf(fout, "repeat_stmt:"); } break;
		case while_stmt: { fprintf(fout, "while_stmt:"); } break;
		case goto_stmt:	{ fprintf(fout, "goto: %d", p->attr.intVal); } break;
		case proc_stmt: { fprintf(fout, "procedure name:%s", p->name); } break;
		case assign_stmt: { fprintf(fout, "assign_stmt:"); } break;
		case case_stmt: { fprintf(fout, "case_stmt:"); } break;
		case case_exp_stmt: { fprintf(fout, "case:"); } break;
	}
}
void print_sub(TreeNode *p){
	switch (p->sub){
		case func_kind: { fprintf(fout, "function %s:", p->name); } break;
		case proc_kind: { fprintf(fout, "procedure %s:", p->name); } break;
		case param_var_kind: { fprintf(fout, "var param:"); } break;
		case param_val_kind: { fprintf(fout, "val param:"); } break;
	}
}
void print_tree(TreeNode *p, int level){
	int i;	
	if (!p) return;
	else fprintf(fout, "\n%d\t", p->lineNo);
	for (i=0; i<level; i++) fprintf(fout, "  ");
	fprintf(fout, "|-");
	switch (p->node){
		case routine_kind: { fprintf(fout, "routine:"); } break;
		case const_kind: { print_const(p); } break;
		case type_kind: { print_type(p); } break;
		case var_kind: { print_var(p); } break;
		case expr_kind: { print_expr(p); } break;
		case stmt_kind: { print_stmt(p); } break;
		case sub_kind: { print_sub(p); } break;
	}
	print_tree(p->dtype, level+1);
	print_tree(p->children[0], level+1); print_tree(p->children[1], level+1);
	print_tree(p->children[2], level+1); print_tree(p->children[3], level+1);
	print_tree(p->sibling, level);
}

