#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"
#include "SyntaxTree.h"
/* error handler*/
void throw_error(string s, TreeNode *p){
	fprintf(stderr, "line %d: %s: %s\n", p->lineNo, s.c_str(), p->name);
}
/* hash function */
#define SIZE 256
#define SHIFT 4
int myhash(char *key){
	int ret = 0;
	int i = 0;
	while (key[i]!='\0'){
		ret = ((ret << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return ret;
}
/* Create Symbol Table*/
SymbolTable *CreateSymbolTable(char *path, int nestLevel){
	int i;
	SymbolTable *p = (SymbolTable *)malloc(sizeof(SymbolTable));
	strcpy(p->path, path);
	p->nestLevel = nestLevel;
	p->sibling = NULL;
	for (i=0; i<256; i++){
		p->NameList[i] = NULL;
	}
}
int ExistSymbolTable(char *path){
	SymbolTable *p = SymTab;
	while (p){
		if (!strcmp(p->path, path)) return 1;
		p = p->sibling;
	}
	return 0;
}
/* Create Symbol element */
void InsertSymbol(char *name, TreeNode *node, SymbolTable *symtab){
	int idx = myhash(name);
	Symbol *q = symtab->NameList[idx];
	/* init p */
	Symbol *p = (Symbol *)malloc(sizeof(Symbol));
	strcpy(p->name, name);
	p->content = node;
	/* connect nodes */
	if (q){
		while (q->next) q = q->next;
		q->next = p;
	} else {
		symtab->NameList[idx] = p;
	}
}
/* Insert element */
void Insert(char *name, char *path, TreeNode *node){
	SymbolTable *p = SymTab;
	while (p){
		if (!strcmp(p->path, path)){
			InsertSymbol(name, node, p);
		}
		p = p->sibling;
	}
}
/* lookup an element */
int sub_str(char *src, char *dst){
	int i=0, j=0;
	while (src[i] && (src[i]==dst[j])){ i++; j++; };
	if (!src[i]) return 1;
	else return 0;
}
/* lookup in a symbol table */
TreeNode *LookupSymbol(char *name, SymbolTable *symtab){
	int idx = myhash(name);
	Symbol *p = symtab->NameList[idx];
	while (p){
		if (!strcmp(p->name, name)){
			return p->content;
		}
		p = p->next;
	}
	return NULL;
}
/* lookup in all symbol tables */
TreeNode *Lookup(char *name, char *path){
	SymbolTable *p = SymTab;
	int ansLevel = -1;
	TreeNode *ret = NULL;
	TreeNode *tmp = NULL;
	// printf("lookup:%s, path:%s\n", name, path);
	while (p){
		if (sub_str(p->path, path)){
			tmp = LookupSymbol(name, p);
			if (tmp){
				if (ansLevel<p->nestLevel){
					ret = tmp;
					ansLevel = p->nestLevel;
				}
			}
		}
		if (!strcmp(p->path, path)) break;
		p = p->sibling;
	}
	/*
	if (ret){
		switch (ret->simple){
			case integer_type: printf("find %s: integer\n", name); break;
			case real_type: printf("find %s: real\n", name); break;
			case char_type: printf("find %s: char\n", name); break;
			case boolean_type: printf("find %s: boolean\n", name); break;
			case string_type: printf("find %s: string\n", name); break;
		}
	}
	*/
	return ret;
}

void ConnectSymtabs(SymbolTable *p1, SymbolTable *p2){
	while (p1->sibling)
		p1 = p1->sibling;	
	p1->sibling = p2;
}

int DuplicateIdentifier(char *name, char *path){
	SymbolTable *p = SymTab;
	int ansLevel = -1;
	TreeNode *tmp = NULL;
	// printf("lookup:%s, path:%s\n", name, path);
	while (p){
		if (!strcmp(p->path, path)){
			tmp = LookupSymbol(name, p);
			// if (tmp) printf("duplicate %s\n", name);
			break;
		}
		p = p->sibling;
	}
	if (tmp)
		return 1;
	else
		return 0;
}

void SymtabTraverseTree(TreeNode *p, char *path, int nestLevel, SymbolTable *symtab){
	char local_path[256];
	if (!p) return ;
	// printf("path:%s\n", path);
	switch (p->node){
		case routine_kind: {
			SymtabTraverseTree(p->children[0], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[1], path, nestLevel, symtab);
			break;
		}
		case const_kind: {
			if (DuplicateIdentifier(p->name, path))
				throw_error("duplicate identifier", p);
			InsertSymbol(p->name, p, symtab);
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		} 
		case type_kind: {
			switch (p->type){
				case decl_type:{
					if (DuplicateIdentifier(p->name, path))
						throw_error("duplicate identifier", p);
					InsertSymbol(p->name, p, symtab);
					if(p->dtype->type!=record_type){
						SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
					} else {
						sprintf(local_path, "%s/%s", path, p->name);
						SymbolTable *new_symtab = CreateSymbolTable(local_path, nestLevel+1);
						ConnectSymtabs(symtab, new_symtab);
						SymtabTraverseTree(p->dtype, local_path, nestLevel+1, new_symtab);
						SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
					}
					break;
				}
				case record_type:{
					SymtabTraverseTree(p->children[0], path, nestLevel, symtab);
					break;
				}
				default:{ break; }
			}
			break;
		}
		case var_kind:{
			TreeNode *q;
			q = p->children[0];
			while (q){
				if (DuplicateIdentifier(q->name, path))
					throw_error("duplicate identifier", q);
				InsertSymbol(q->name, p, symtab);
				q = q->sibling;
			}
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		} 
		case sub_kind: {
			switch(p->sub){
				case func_kind:{
					// printf("enter function %s decl:\n", p->name);
					InsertSymbol(p->name, p, symtab);
					sprintf(local_path, "%s/%s", path, p->name);
					SymbolTable * new_symtab = CreateSymbolTable(local_path, nestLevel+1);
					ConnectSymtabs(symtab, new_symtab);
					SymtabTraverseTree(p->children[0], local_path, nestLevel+1, new_symtab);
					SymtabTraverseTree(p->children[1], local_path, nestLevel+1, new_symtab);
					SymtabTraverseTree(p->children[2], local_path, nestLevel+1, new_symtab);
					break;
				}
				case proc_kind:{
					// printf("enter procedure %s decl:\n", p->name);
					InsertSymbol(p->name, p, symtab);
					sprintf(local_path, "%s/%s", path, p->name);
					SymbolTable * new_symtab = CreateSymbolTable(local_path, nestLevel+1);
					ConnectSymtabs(symtab, new_symtab);
					SymtabTraverseTree(p->children[0], local_path, nestLevel+1, new_symtab);
					SymtabTraverseTree(p->children[1], local_path, nestLevel+1, new_symtab);
					SymtabTraverseTree(p->children[2], local_path, nestLevel+1, new_symtab);
					break;
				}
				case param_var_kind:{
					TreeNode *q;
					q = p->children[0];
					while (q){
						if (DuplicateIdentifier(q->name, path))
							throw_error("duplicate identifier", q);
						InsertSymbol(q->name, p, symtab);
						q = q->sibling;
					}
					break;
				}
				case param_val_kind:{
					TreeNode *q;
					q = p->children[0];
					while (q){
						if (DuplicateIdentifier(q->name, path))
							throw_error("duplicate identifier", q);
						InsertSymbol(q->name, p, symtab);
						q = q->sibling;
					}
					break;
				}
			}
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		};
		case expr_kind:{
			switch (p->expr){
				case id_kind:{
					// printf("look for identifier: %s\n", p->name);
					if (!Lookup(p->name, path))
						throw_error("unknown identifier", p);
					break;
				}
				case fn_kind:{
					// printf("look for function: %s\n", p->name);
					if (!Lookup(p->name, path) && !p->system)
						throw_error("unknown identifier", p);
				}
				default: break;
			}
			SymtabTraverseTree(p->children[0], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[1], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[2], path, nestLevel, symtab);
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		}
		case stmt_kind:{
			switch (p->stmt){
				case for_stmt:{
					if (!Lookup(p->name, path))
						throw_error("unknown identifier", p);
				}
				case proc_stmt:{
					if (!Lookup(p->name, path) && !p->system)
						throw_error("unknown identifier", p);
				}
			}
			SymtabTraverseTree(p->children[0], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[1], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[2], path, nestLevel, symtab);
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		}
		default: {
			SymtabTraverseTree(p->children[0], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[1], path, nestLevel, symtab);
			SymtabTraverseTree(p->children[2], path, nestLevel, symtab);
			SymtabTraverseTree(p->sibling, path, nestLevel, symtab);
			break;
		}
	}
}

void print_symtab(SymbolTable *p){
	int i;	
	Symbol *q;
	while (p){
		printf("%s:\n", p->path);
		for (i=0; i<SIZE; i++){
			q = p->NameList[i];
			while (q){
				if (q->content->dtype)
					printf("%s\n", q->name);
				q = q->next;
			}
		}
		p = p->sibling;
	}
}

void BuildSymTab(){
	strcpy(path, "global");
	nestLevel = 0;
	SymTab = CreateSymbolTable(path, nestLevel);
	SymtabTraverseTree(root, path, nestLevel, SymTab);
}
