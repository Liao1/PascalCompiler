#include "MiddleWare.h"

/* 创建常量声明节点 */
ConstAST *CreateConstAST(TreeNode *p){
	string name(p->name);
	BasicTypeAST * type = CreateTypeAST(p->dtype);
	p = p->children[0];
	constValue val;
	switch (p->dtype->type){
		case integer_type: case boolean_type: case char_type:
			val.i = p->attr.intVal;
			val.b = p->attr.intVal;
			val.c = p->attr.intVal;
			return new ConstAST(name, type, val);
		case real_type:
			val.d = p->attr.realVal;
			return new ConstAST(name, type, val);
		case string_type:
			val.s = string(p->attr.strVal);
			return new ConstAST(name, type, val);
		default: return NULL;
	}
}
/* 创建类型声明节点 */
BasicTypeAST *CreateTypeAST(TreeNode *p){
	// cout<<"create type ast"<<endl;
	if (!p) return NULL;
	switch (p->type){
		case integer_type:
			// cout<<"integer"<<endl;
			return new BasicTypeAST(Integer);
		case real_type:
			// cout<<"real"<<endl;
			return new BasicTypeAST(Real);
		case char_type:
			// cout<<"char"<<endl;
			return new BasicTypeAST(Char);
		case boolean_type:
			// cout<<"boolean"<<endl;
			return new BasicTypeAST(Bool);
		case string_type:
			// cout<<"string"<<endl;
			return new BasicTypeAST(String);
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
		// cout<<q->name<<endl;
		q = q->sibling;
	}
	return new VariableDeclAST(CreateTypeAST(p->dtype), names);
}
/* 创建函数声明节点 */
/*PrototypeAST * CreatePrototypeAST(TreeNode *p){
	vector <VariableDeclAST *> decl;
	string name(p->name);
	TreeNode *q = p->children[0];
	TreeNode *type = p->dtype;
	while (q){
		TreeNode *qchildren = q->children[0];
		while (qchildren){
			VariableDeclAST *tmp = new VariableDeclAST(
				qchildren->name, 
				CreateTypeAST(dtype),
				);
			decl.push_back(tmp);
			qchildren = qchildren->sibling;
		}
		q = q->sibling;
	}
	return new PrototypeAST(name, decl, CreateTypeAST(type));
}*/
/* 创建表达式节点 */
ExprAST *CreateExprAST(TreeNode *p){
	if (!p) return NULL;
	// cout<<"create expr"<<endl;
	ExprAST *left = CreateExprAST(p->children[0]);
	ExprAST *right = CreateExprAST(p->children[1]);
	ExprAST *tmp;
	switch (p->expr){
		case op_kind:{
			switch (p->op){
				case plus_kind: {
					tmp = new BinaryExprAST(plusKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case minus_kind: {
					tmp = new BinaryExprAST(minusKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case or_kind: {
					tmp = new BinaryExprAST(orKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case mul_kind: {
					tmp = new BinaryExprAST(mulKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case div_kind: {
					tmp = new BinaryExprAST(divKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case mod_kind: {
					tmp = new BinaryExprAST(modKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case and_kind: {
					tmp = new BinaryExprAST(andKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case ge_kind: {
					tmp = new BinaryExprAST(geKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case gt_kind: {
					tmp = new BinaryExprAST(gtKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case le_kind: {
					tmp = new BinaryExprAST(leKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case lt_kind: {
					tmp = new BinaryExprAST(ltKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case eq_kind: {
					tmp = new BinaryExprAST(eqKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case ueq_kind: {
					tmp = new BinaryExprAST(ueqKind, left, right);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case not_kind: {
					tmp = new UnaryExprAST(notKind, left);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
				case neg_kind: {
					tmp = new UnaryExprAST(negKind, left);
					tmp->expr_type = BinaryExpr;
					return tmp;
				}
			}
		}
		case id_kind:{
			switch (p->id){
				case Basic:
					tmp = new VariableExprAST(p->name);
					tmp->expr_type = Variable;
					return tmp;
				case Array:
					return NULL;
					/*return new ArrayVariableExprAST(
						p->name,
						CreateExprAST(p->children[0])
						);*/
				case Record:
					return NULL;
			}
		}
		case con_kind:{
			switch (p->dtype->type){
				case integer_type:
					tmp = new NumberExprAST(p->attr.intVal);
					tmp->expr_type = NumberExpr;
					return tmp;
				case real_type:
					tmp = new RealExprAST(p->attr.realVal);
					tmp->expr_type = RealExpr;
					return tmp;
				case boolean_type:
					tmp = new BoolExprAST(p->attr.intVal);
					tmp->expr_type = BoolExpr;
					return tmp;
				case char_type:
					tmp = new CharExprAST(p->attr.intVal);
					tmp->expr_type = CharExpr;
					return tmp;
				case string_type:
					tmp = new StringExprAST(p->attr.strVal);
					tmp->expr_type = StringExpr;
					return tmp;
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
			tmp = new CallFunctionExprAST(p->name, args, p->system);
			tmp->expr_type = CallExpr;
			return tmp;
		}
	}
}
ExprAST *CreateStmtExprAST(TreeNode *p){
	// cout<<"createStmt"<<endl;
	if (!p) return NULL;
	ExprAST *tmp;
	switch (p->stmt){
		case assign_stmt:{
			tmp = new BinaryExprAST(
				assignmentKind,
				CreateExprAST(p->children[0]),
				CreateExprAST(p->children[1])
				);
			tmp->expr_type = BinaryExpr;
			return tmp;
		}
		case proc_stmt:{
			std::vector<ExprAST *>args;
			TreeNode *q = p->children[0];
			while (q){
				args.push_back(CreateExprAST(q));
				q = q->sibling;
			}
			tmp = new CallProcedureExprAST(p->name, args, p->system);
			tmp->expr_type = CallExpr;
			return tmp;
		}
		case if_stmt:{
			vector<ExprAST *>thenComp, elseComp;
			TreeNode *q = p->children[1];
			while (q){
				thenComp.push_back(CreateStmtExprAST(q));
				q = q->sibling;
			}
			q = p->children[2];
			while (q){
				elseComp.push_back(CreateStmtExprAST(q));
				q = q->sibling;
			}
			tmp = new IfExprAST(CreateExprAST(p->children[0]), thenComp, elseComp);
			return tmp;
		}
		case for_stmt: {
			vector<ExprAST *> execComp;
			TreeNode *q = p->children[2];
			while (q){
				execComp.push_back(CreateStmtExprAST(q));
				q = q->sibling;
			}
			tmp = new ForExprAST(
				string(p->name),
				CreateExprAST(p->children[0]),
				CreateExprAST(p->children[1]),
				execComp, p->attr.intVal);
			return tmp;
		}
		default: 
			return NULL;	
	}
}
/* 创建函数过程节点 */
FunctionAST *CreateFunctionAST(TreeNode *p){
	//PrototypeAST *proto;
	// cout<<"createFunction"<<endl;
	string name(p->name);
	vector <VariableDeclAST *> args;
	std::vector<ConstAST *> consts;
	std::vector<SelfdefineTypeAST *> type;
	std::vector<VariableDeclAST *> decl;
	std::vector<FunctionAST *> functions;
	std::vector<ExprAST *>body;
	BasicTypeAST *returnType;
	//proto = CreatePrototypeAST(p);
	TreeNode *q = p->children[0];
	//TreeNode *type = p->dtype;
	// cout<<"create args"<<endl;
	while (q){
		VariableDeclAST *tmp = CreateVariableAST(q);
		args.push_back(tmp);
		q = q->sibling;
	}
	// cout<<"create return_type"<<endl;
	returnType = CreateTypeAST(p->dtype);
	/* routine head part */
	q = p->children[1]->children[0];
	while (q){
		switch (q->node){
			case const_kind:{
				// cout<<"create const"<<endl;
				consts.push_back(CreateConstAST(q)); 
				break;
			}
			case type_kind:{
				// cout<<"create type"<<endl;
				//type.push_back(CreateTypeAST(q));
				break;
			}
			case var_kind:{
				// cout<<"create var"<<endl;
				VariableDeclAST *tmp;
				tmp = CreateVariableAST(q);
				if (name=="main")
					tmp->isGlobal = 1;
				else
					tmp->isGlobal = 0;
				decl.push_back(tmp); 
				break;
			}
			case sub_kind:{
				// cout<<"create function"<<endl;
				functions.push_back(CreateFunctionAST(q)); 
				break;
			} 
		}
		q = q->sibling;
	}
	// cout<<"create stmt"<<endl;
	/* routine body part */
	q = p->children[1]->children[1];
	while (q){
		body.push_back(CreateStmtExprAST(q));
		q = q->sibling;
	}
	if (p->sub==func_kind)
		return new FunctionAST(name, args, returnType, consts, type, decl, functions, body, type_function);
	else 
		return new FunctionAST(name, args, returnType, consts, type, decl, functions, body, type_procedure);
}
