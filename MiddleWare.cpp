#include "MiddleWare.h"

/* 创建常量声明节点 */
ConstAST *CreateConstAST(TreeNode *p){
	string name(p->name);
	BasicTypeAST * type = CreateTypeAST(p->dtype);
	switch (p->dtype->type){
		case integer_type: case boolean_type: case char_type:
			return new ConstAST(name, type, p->attr.intVal);
		case real_type:
			return new ConstAST(name, type, p->attr.realVal);
		case string_type:
			return new ConstAST(name, type, p->attr.strVal);
		default: return NULL;
	}
}
/* 创建类型声明节点 */
TypeAST *CreateTypeAST(TreeNode *p){
	switch (p->type){
		case integer_type:
			return new BasicTypeAST(INTEGER);
		case real_type:
			return new BasicTypeAST(REAL);
		case char_type:
			return new BasicTypeAST(CHAR);
		case boolean_type:
			return new BasicTypeAST(BOOL);
		case string_type:
			return new BasicTypeAST(STRING);
		default:
			return NULL;
	}
}
/* 创建变量声明节点 */
VariableDeclAST * CreateVariableAST(TreeNode *p){
	TreeNode *q = p->children[0];
	vector<string> names;
	while (q){
		names.push_back(q->name);
		q = q->sibling;
	}
	return new VariableDeclAST(CreateTypeAST(p->dtype), names);
}
/* 创建函数声明节点 */
PrototypeAST * CreatePrototypeAST(TreeNode *p){
	vector <VariableDeclAST *> decl;
	string name(p->name);
	TreeNode *q = p->children[0];
	TreeNode *type = p->dtype;
	while (q){
		TreeNode *qchildren = q->children[0];
		while (qchildren){
			VariableDeclAST *tmp = new VariableDeclAST(
				qchildren->name, 
				new CreateTypeAST(dtype)
				);
			decl.push_back(tmp);
			qchildren = qchildren->sibling;
		}
		q = q->sibling;
	}
}
/* 创建表达式节点 */
ExprAST *CreateExprAST(TreeNode *p){
	if (!p) return NULL;
	ExprAST *left = CreateExprAST(p->children[0]);
	ExprAST *right = CreateExprAST(p->children[1]);
	switch (p->expr){
		case op_kind:{
			switch (p->op){
				case plus_kind: {
					return new BinaryExprAST(plusKind, left, right);
				}
				case minus_kind: {
					return new BinaryExprAST(minusKind, left, right);
				}
				case or_kind: {
					return new BinaryExprAST(orKind, left, right);
				}
				case mul_kind: {
					return new BinaryExprAST(mulKind, left, right);
				}
				case div_kind: {
					return new BinaryExprAST(divKind, left, right);
				}
				case mod_kind: {
					return new BinaryExprAST(modKind, left, right);
				}
				case and_kind: {
					return new BinaryExprAST(andKind, left, right);
				}
				case ge_kind: {
					return new BinaryExprAST(geKind, left, right);
				}
				case gt_kind: {
					return new BinaryExprAST(gtKind, left, right);
				}
				case le_kind: {
					return new BinaryExprAST(leKind, left, right);
				}
				case lt_kind: {
					return new BinaryExprAST(ltKind, left, right);
				}
				case eq_kind: {
					return new BinaryExprAST(eqKind, left, right);
				}
				case ueq_kind: {
					return new BinaryExprAST(ueqKind, left, right);
				}
				case not_kind: {
					return new UnaryExprAST(notKind, left);
				}
				case neg_kind: {
					return new UnaryExprAST(negKind, left);
				}
			}
		}
		case id_kind:{
			switch (p->id){
				case basic:
					return new VariableExprAST(p->name);
				case array:
					return new ArrayVariableExprAST(
						p->name,
						CreateExprAST(p->children[0])
						);
				case record:
					return NULL;
			}
		}
		case con_kind:{
			switch (p->dtype->type){
				case integer_type:
					return new NumberExprAST(p->attr.intVal);
				case real_type:
					return new RealExprAST(p->attr.realVal);
				case boolean_type:
					return new BoolExprAST(p->attr.intVal);
				case char_type:
					return new CharExprAST(p->attr.intVal);
				case string_type:
					return new StringExprAST(p->attr.strVal);
				default:
					return NULL;
			}
		}
		case  fn_kind:{
			std::vector<ExprAST *>args;
			TreeNode *q = p->children[0];
			while (q){
				args.push_back(CreateExprAST(q));
				q = q->sibling;
			}
			return new CallExprAST(p->name, args, p->system);
		}
	}
}
ExprAST *CreateStmtExprAST(TreeNode *p){
	switch (p->stmt){
		case assign_stmt:{
			return new BinaryExprAST(
				assignmentKind,
				CreateExprAST(p->children[0]),
				CreateExprAST(p->children[1])
				);
		}
		default: 
			return NULL;	
	}
}
/* 创建函数过程节点 */
FunctionAST *CreateFunctionAST(TreeNode *p){
	PrototypeAST *proto;
	std::vector<ConstAST *> consts;
	std::vector<SelfdefineTypeAST *> type;
	std::vector<VariableDeclAST *> decl;
	std::vector<FunctionAST *> functions;
	std::vector<ExprAST *>body;
	proto = CreatePrototypeAST(p);
	proto->returnType = CreateTypeAST(p->dtype);
	/* routine head part */
	q = p->children[0];
	while (q){
		switch (q->node){
			case const_kind:{
				consts.push_back(CreateConstAST(q)); 
				break;
			}
			case type_kind:{
				//type.push_back(CreateTypeAST(q));
				break;
			}
			case var_kind:{
				str::vector<VariableDeclAST *>tmp;
				tmp = CreateVariableAST(p);
				for (int i=0; i<tmp.size(); i++){
					decl.push_back(tmp[i]);
				} 
				break;
			}
			case sub_kind:{
				functions.push_back(CreateFunctionAST(q)); 
				break;
			} 
		}
		q = q->sibling;
	}
	/* routine body part */
	vector<ExprAST *> body;
	q = p->chilren[1];
	while (q){
		body.push_back(CreateStmtExprAST(q));
		q = q->sibling;
	}
	return new FunctionAST(proto, consts, type, decl, functions, body);
}
