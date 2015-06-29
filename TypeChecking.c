#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"
#include "SyntaxTree.h"
#include "TypeChecking.h"


void evalType(TreeNode *p, char *path){
	char local_path[256];
	if (!p) return;
	switch (p->node){
		case routine_kind: {
			//printf("eval routine\n");
			evalType(p->children[0], path);
			break;
		}
		case const_kind: {
			//printf("eval const: %s\n", p->name);
			p->dtype = p->children[0]->dtype;
			evalType(p->sibling, path);
			break;
		}
		case type_kind: {
			//printf("eval type: %s\n", p->name);
			evalType(p->sibling, path);
			break;
		}
		case var_kind: {
			//printf("eval var\n");
			//if (p->dtype)
			//	printf("has type\n");
			p->children[0]->dtype = p->dtype;
			evalType(p->children[0], path);
			evalType(p->sibling, path);
			break;
		} 
		case sub_kind: {
			switch (p->sub){
				case func_kind: case proc_kind: {
					//printf("eval sub: %s\n", p->name);
					sprintf(local_path, "%s/%s", path, p->name);
					evalType(p->children[0], local_path);
					evalType(p->children[1], local_path);
					evalType(p->sibling, path);
					break;
				}
				default: {
					//printf("eval sub param\n");
					p->children[0]->dtype = p->dtype;
					evalType(p->children[0], path);
					break;
				}
			}
			break;
		}
		case expr_kind: {
			switch (p->expr){
				case id_kind: {
					//printf("eval id: %s\n", p->name);
					//if (p->dtype)
					//	printf("has type\n");
					if (p->sibling){
						//printf("sibling id: %s\n", p->sibling->name);
						p->sibling->dtype = p->dtype;
						evalType(p->sibling, path);
					}
					break;
				}
				default: break; 
			}
			break;
		}
		default: {
			printf("failed!\n");
			break;
		}
	}
}

int SimpleType(TreeNode *p){
	switch(p->type){
		case integer_type:
		case real_type:
		case char_type:
		case string_type:
		case boolean_type: return 1;
		default: return 0;
	}
}

int is_integer(TreeNode *p){
	if (p->dtype->type==integer_type)
		return 1;
	else 
		return 0;
}

// int typeEqual(TreeNode *p, TreeNode *q, char *path){
// 	/* if p or q's type is selfdefined, get the real type */
// 	while (p->type==selfdefined_type)
// 		p = Lookup(p->name, path);
// 	while (q->type==selfdefined_type)
// 		q = Lookup(p->name, path);
// 	/* start with simple type */
// 	if (SimpleType(p) && SimpleType(q)) 
// 		return p->type==q->type;
// 	else if (p->type==array_type && q->type==array_type){
// 		return typeEqual(p->children[0], q->children[0], path) && typeEqual(p->children[1], q->children[1], path);
// 	} else if (p->type==record_type && q->type==record_type){
// 		TreeNode *pchild, *qchild;
// 		int tmp = 1;
// 		pchild = p->children[0]; qchild = q->children[0];
// 		while (tmp && pchild && qchild){
// 			TreeNode *pfield, *qfield;
// 			pfield = pchild->children[0]; qfield = qchild->children[0];
// 			while (tmp && pfield && qfield){
// 				if (strcmp(pfield->name, qfield->name))
// 					tmp = 0;
// 				if (!typeEqual(pfield->dtype, qfield->dtype))
// 					tmp = 0;
// 				pfield = pfield->sibling; qfield = qfield->sibling;
// 			}
// 			if (pfield || qfield) tmp = 1;
// 			pchild = pchild->sibling; qchild = qchild->sibling;
// 		}
// 		if (pchild || qchild) tmp = 1;
// 		return tmp;
// 	} else if (p->type==enum_type && q->type==enum_type){
// 		if (p->children[1] && q->children[1]){
// 			/* exp..exp */
// 			return (p->children[0]->attr.intVal == q->children[0]->attr.intVal) && 
// 					(p->children[1]->attr.intVal == q->children[1]->attr.intVal);
// 		} else if (!p->children[1] && !p->children[0]){
// 			/* (name1, name2, name3, ...) */
// 			TreeNode *pchild, *qchild;
// 			pchild = p->children[0]; qchild = q->children[0];
// 			int tmp = 1;
// 			while (tmp && pchild && qchild){
// 				if (strcmp(pchild->name, qchild->name))
// 					tmp = 0;
// 				pchild = pchild->sibling; qchild = qchild->sibling;
// 			}
// 			if (pchild || qchild) tmp = 0;
// 			return tmp;
// 		}
// 	} else 
// 		return 0;
// }

// void checkStmt(TreeNode *p, char *path){
// 	char local_path[256];
// 	if (!p) return;
// 	switch (p->node){
// 		case routine_kind: {
// 			printf("eval routine\n");
// 			checkStmt(p->children[0], path);
// 			checkStmt(p->children[1], path);
// 			break;
// 		}
// 		case const_kind: {
// 			checkStmt(p->sibling, path);
// 			break;
// 		}
// 		case type_kind: {
// 			checkStmt(p->sibling, path);
// 			break;
// 		}
// 		case var_kind: {
// 			checkStmt(p->sibling, path);
// 			break;
// 		} 
// 		case sub_kind: {
// 			switch (p->sub){
// 				case func_kind: case proc_kind: {
// 					checkStmt(p->children[0], local_path);
// 					checkStmt(p->children[1], local_path);
// 					checkStmt(p->sibling, path);
// 					break;
// 				}
// 				default: {
// 					checkStmt(p->children[0], path);
// 					break;
// 				}
// 			}
// 			break;
// 		}
// 		case stmt_kind: {
// 			checkStmt(p->children[0]);
// 			checkStmt(p->children[1]);
// 			checkStmt(p->children[2]);
// 			switch (p->stmt){
// 				case assign_stmt: {

// 				}
// 				case proc_stmt: 
// 				case if_stmt: 
// 				case repeat_stmt: 
// 				case while_stmt: 
// 				case for_stmt: 
// 				case case_stmt: 
// 				case goto_stmt:
// 				case case_exp_stmt: 
// 				default: break;
// 			}
// 		}
// 		case expr_kind: {
// 			switch (p->expr){
// 				case op_kind: {
// 					switch (p->op){

// 					}
// 				}
// 				case id_kind:
// 				case fn_kind:
// 				case con_kind:
// 				default: break; 
// 			}
// 			break;
// 		}
// 		default: {
// 			printf("failed!\n");
// 			break;
// 		}
// 	}
// }


void TypeChecking(TreeNode *root){
	char path[256];
	strcpy(path, "global");
	evalType(root, path);
}