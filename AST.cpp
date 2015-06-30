#include "node.h"
#include <iostream>
using namespace std;
using namespace llvm;

void printTab(int n){
	for (int i=0; i<n; i++)
		cout<<"\t";
}

void FunctionAST::print(int n){
	printTab(n);
	if (isFunction())
		cout<<"function:"<<name<<endl;
	else 
		cout<<"procedure:"<<name<<endl;
	for (int i=0; i<headerDecl.size(); i++)
		headerDecl[i]->print(n+1);
	if (returnType)
		returnType->print(n+1);
	for (int i=0; i<consts.size(); i++)
		consts[i]->print(n+1);
	for (int i=0; i<bodyDecl.size(); i++)
		bodyDecl[i]->print(n+1);
	for (int i=0; i<functions.size(); i++)
		functions[i]->print(n+1);
	for (int i=0; i<body.size(); i++)
		body[i]->print(n+1);
	printTab(n);
	cout<<"end"<<endl;
}
/*
void PrototypeAST::print(int n){
	printTab(n);
	cout<<"Prototype:"<<name<<endl;
	for (int i=0; i<args.size(); i++){
		args[i]->print(n+1);
	}
	if (returnType){
		returnType->print(n+1);
	}
}
*/
void CallFunctionExprAST::print(int n){
	printTab(n);
	if (isSystemCall)
		cout<<"SysCall("<<callee<<")function"<<endl;
	else
		cout<<"Call("<<callee<<")function"<<endl;
	for (int i=0; i<args.size(); i++){
		printTab(n);
		args[i]->print(n+1);
	}
}

void CallProcedureExprAST::print(int n){
	printTab(n);
	if (isSystemCall)
		cout<<"SysCall("<<callee<<")procedure"<<endl;
	else
		cout<<"Call("<<callee<<")procedure"<<endl;
	for (int i=0; i<args.size(); i++){
		printTab(n);
		args[i]->print(n+1);
	}
}

void BinaryExprAST::print(int n){
	printTab(n);
	cout<<"operation:";
	switch (op){
		case plusKind: cout<<"plus"<<endl; break;
		case minusKind: cout<<"minus"<<endl; break;
		case orKind: cout<<"or"<<endl; break;
		case mulKind: cout<<"mul"<<endl; break;
		case divKind: cout<<"div"<<endl; break;
		case modKind: cout<<"mod"<<endl; break;
		case andKind: cout<<"and"<<endl; break;
		case geKind: cout<<"ge"<<endl; break;
		case gtKind: cout<<"gt"<<endl; break;
		case leKind: cout<<"le"<<endl; break;
		case ltKind: cout<<"lt"<<endl; break;
		case eqKind: cout<<"eq"<<endl; break;
		case ueqKind: cout<<"ueq"<<endl; break;
		case assignmentKind: cout<<"assign"<<endl; break;
	}
	LExpr->print(n+1);
	RExpr->print(n+1);
}

void UnaryExprAST::print(int n){
	printTab(n);
	cout<<"operation";
	switch (op){
		case notKind: cout<<"not"<<endl;
		case negKind: cout<<"neg"<<endl;
	}
	expr->print(n+1);
}

void VariableExprAST::print(int n){
	printTab(n);
	cout<<"variable:"<<name<<endl;
}

void CharExprAST::print(int n){
	printTab(n);
	cout<<"const char:"<<val<<endl;
}

void StringExprAST::print(int n){
	printTab(n);
	cout<<"const string:"<<val<<endl;
}

void BoolExprAST::print(int n){
	printTab(n);
	cout<<"const bool"<<val<<endl;
}

void RealExprAST::print(int n){
	printTab(n);
	cout<<"const real:"<<val<<endl;
}

void NumberExprAST::print(int n){
	printTab(n);
	cout<<"const integer:"<<val<<endl;
}

void VariableDeclAST::print(int n){
	printTab(n);
	if (isGlobal)
		cout<<"global var:"<<endl;
	else 
		cout<<"local var:"<<endl;
	// if (!type) cout<<"var null"<<endl;
	// cout<<variableName.size()<<endl;
	type->print(n+1);
	printTab(n+1);
	for (int i=0; i<variableName.size(); i++){
		cout<<variableName[i]<<",";
	}
	cout<<endl;
}

void ConstAST::print(int n){
	printTab(n);
	cout<<"const:"<<variableName<<endl;
	type->print(n+1);
	printTab(n+1);
	cout<<"value:";
	switch (type->type){
		case Integer: cout<<value.i<<endl; break;
		case Real: cout<<value.d<<endl; break;
		case String: cout<<value.s<<endl; break;
		case Char: cout<<value.c<<endl; break;
		case Bool: cout<<value.b<<endl; break;
	}
}

void BasicTypeAST::print(int n){
	printTab(n);
	cout<<"type:";
	switch (type){
		case Integer: cout<<"integer"<<endl; break;
		case Real: cout<<"real"<<endl; break;
		case String: cout<<"string"<<endl; break;
		case Char: cout<<"char"<<endl; break;
		case Bool: cout<<"boolean"<<endl; break;
	}
}

void IfExprAST::print(int n){
	printTab(n);
	cout<<"if stmt:"<<endl;
	printTab(n+1);
	cout<<"if expr:"<<endl;
	ifCond->print(n+1);
	printTab(n+1);
	cout<<"then stmt:"<<endl;
	for (int i=0; i<thenComponent.size(); i++)
		thenComponent[i]->print(n+1);
	printTab(n+1);
	cout<<"else stmt:"<<endl;
	for (int i=0; i<elseComponent.size(); i++)
		elseComponent[i]->print(n+1);
}

void ForExprAST::print(int n){
	printTab(n);
	cout<<"for expr:"<<endl;
	printTab(n+1);
	cout<<"loop var:"<<varName<<endl;
	start->print(n+1);
	printTab(n+1);
	if (increaseDirection)
		cout<<"to"<<endl;
	else 
		cout<<"downto"<<endl;
	end->print(n+1);
	for (int i=0; i<body.size(); i++)
		body[i]->print(n+1);
}