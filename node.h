#ifndef Node_H
#define Node_H

#include "SyntaxTree.h"
#include <vector>
#include <map>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

class constValue;
class Node;
class ExprAST;
class TypeAST;
class NumberExprAST;
class RealExprAST;
class BoolExprAST;
class CharExprAST;
class StringExprAST;
class BasicTypeAST;
class ConstAST;
class VariableDeclAST;
class VariableExprAST;
class UnaryExprAST;
class BinaryExprAST;
class CallExprAST;
class FunctionAST;
class CodeGenContext;

using namespace std;
using namespace llvm;

extern LLVMContext &context;
extern IRBuilder<> builder;
extern Module module;
extern Function *startFunc;
extern string errorMsg;
extern FunctionAST *program;

extern Value* createCast(Value *value,Type *type);
extern Constant* getInitial(Type *type);
extern void throwError(Node *node);
extern void throwWarning(Node *node,string msg);
extern string getOperatorName(int op);
extern string getTypeName(Type *type);


//二元表达式集合
typedef enum {
	plusKind,	// plus
	minusKind,	// minus
	orKind,	// or
	mulKind,	// muliply
	divKind,	// div
	modKind,	// mod
	andKind,	// and
	geKind,	// greater equal
	gtKind,	// greater than
	leKind,	// less equal
	ltKind,	// less than
	eqKind,	// equal
	ueqKind,	// unequal
	assignmentKind	// assignment
} BinaryOpKind;

//一元表达式集合
typedef enum  {
	notKind,	// not
	negKind	// negative
} UnaryOpKind;

//const值类型
class constValue {
public:
	int i;
	double d;
	char c;
	string s;
	bool b;
	constValue() {}
	constValue(const constValue &f):i(f.i), d(f.d), c(f.c), s(f.s), b(f.b){}
};

//TypeAST中用来标记是哪种类型
typedef enum {  
	basic_kind,
	array_kind,
	self_kind
} TypeName;

//SelfdefineTypeAST中用来标记类型
typedef enum {  
	record,
	changeName
} SelfType;

//BasicTypeAST中的物种基础类型
typedef enum {  
	Integer,
	String,
	Real,
	Char,
	Bool
} BasicType;

typedef enum {  
	NumberExpr,
	RealExpr,
	BoolExpr,
	StringExpr,
	CharExpr,
	Variable,
	ArrayVariable,
	RecordVariable,
	UnaryExpr,
	BinaryExpr,
	CallExpr,
	IfExpr,
	ForExpr,
	WhileExpr,
	RepeatExpr
} ExprType;

class Node {
public:
	Node(){}
	virtual ~Node(){}
	virtual Value* Codegen(CodeGenContext& astcontext){};
	virtual void print(int i) {};
	// TypeAST* Error(const char *str){fprintf(stderr, "%s\n", str); return 0;}
};

//所有表达式类的基类
class ExprAST : public Node{
public:
	ExprType type;
	ExprAST(){}
	virtual Value* Codegen(CodeGenContext& astcontext){};
	// ExprAST* ErrorE(const char *str){Error(str); return 0;}//这里设置返回，是为了报错时可以扩展为定位到节点而不是简单出现错误信息
};

//所有类型类的基类
class TypeAST : public Node{
public:
	TypeAST(){}
	virtual void print(int i){};
	//TypeName type;
	// TypeAST* ErrorT(const char *str){Error(str); return 0;}
};

//整数常数表达式，如“1”
class NumberExprAST : public ExprAST {
public:
	int val;
	NumberExprAST(int value):val(value){}
	NumberExprAST(const NumberExprAST &f):val(f.val){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// NumberExprAST* ErrorN(const char *str){Error(str); return 0;}
};

//实数常数表达式，如“1.0”
class RealExprAST : public ExprAST {
public:
	double val;

	RealExprAST(double value):val(value){}
	RealExprAST(const RealExprAST &f):val(f.val){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// RealExprAST* ErrorR(const char *str){Error(str); return 0;}
};

//布尔常数表达式，如“true”
class BoolExprAST : public ExprAST {
public:
	bool val;

	BoolExprAST(bool value):val(value){}
	BoolExprAST(const BoolExprAST &f):val(f.val){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// BoolExprAST* ErrorB(const char *str){Error(str); return 0;}
};

//字符串常数表达式，如“wasd”
class StringExprAST : public ExprAST {
public:
	string val;

	StringExprAST(const string value):val(value){}
	StringExprAST(const StringExprAST &f):val(f.val){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// StringExprAST* ErrorS(const char *str){Error(str); return 0;}
};

//字符常数表达式，如“a”
class CharExprAST : public ExprAST {
public:
	char val;
	CharExprAST(char value):val(value){}
	CharExprAST(const CharExprAST &f):val(f.val){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// CharExprAST* ErrorC(const char *str){Error(str); return 0;}
};


//基础类型如string、integer等
class BasicTypeAST : public TypeAST {
public:
	BasicType type;

	BasicTypeAST(BasicType type):type(type){}
	BasicTypeAST(const BasicTypeAST &f):type(f.type){}
	void print(int n) override;
	// BasicTypeAST* ErrorB(const char *str){Error(str); return 0;}
};

//const类
class ConstAST : public Node{
public:
	string variableName;
	BasicTypeAST* type;
	constValue value;

	ConstAST(string name, BasicTypeAST* ty, constValue val)
		:variableName(name), type(ty), value(val){}
	ConstAST(const ConstAST &f): variableName(f.variableName), type(f.type), value(f.value){}
	~ConstAST(){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// ConstAST* ErrorC(const char *str){fprintf(stderr, "%s\n", str); return 0;}
};

//数组类型
class ArrayTypeAST : public TypeAST {
public:
	TypeAST *type;
	ConstAST *startIndex, *endIndex;

	ArrayTypeAST(TypeAST *ty, ConstAST *s, ConstAST *e):type(ty), startIndex(s), endIndex(e){}
	//ArrayTypeAST* ErrorA(const char *str){Error(str); return 0;}
};

//自定义类型
class SelfdefineTypeAST : public TypeAST {
public:
	//SelfType type;
	SelfdefineTypeAST(){}
	//virtual ~SelfDefineAST(){}
	//virtual Value* Codegen(CodeGenContext& context) override;
	//SelfdefineTypeAST* ErrorS(const char *str){Error(str); return 0;}
};

//变量声明
class VariableDeclAST : public Node{
public:
	BasicTypeAST *type;
	vector<string> variableName;
	VariableDeclAST(BasicTypeAST *t, vector<string> v):type(t), variableName(v){}
	VariableDeclAST(const VariableDeclAST &f):type(f.type), variableName(f.variableName){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// VariableDeclAST* ErrorN(const char *str){Error(str); return 0;}
};

//record类型
class RecordTypeAST : public SelfdefineTypeAST {
public:
	vector<VariableDeclAST*> declList;

	RecordTypeAST(vector<VariableDeclAST*> v):declList(v){}
	//RecordTypeAST* ErrorR(const char *str){ErrorS(str); return 0;}
	//Value* Codegen(CodeGenContext& context) override;
};

//type A=integer;这种自定义类型
class ChangeNameTypeAST : public SelfdefineTypeAST {
public:
	string newName;
	TypeAST* originalType;

	ChangeNameTypeAST(string name, TypeAST* old):newName(name), originalType(old){}
	//ChangeNameTypeAST* ErrorC(const char *str){ErrorS(str); return 0;}
	//Value* Codegen(CodeGenContext& context) override;
};

//普通变量表达式，如“i”
class VariableExprAST : public ExprAST {
public:
	string name;

	VariableExprAST(const string variableName):name(variableName){}
	VariableExprAST(const VariableExprAST &f):name(f.name){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// VariableExprAST* ErrorV(const char *str){Error(str); return 0;}
};

//数组变量表达式，如"a[i]"
class ArrayVariableExprAST : public ExprAST {
public:
	string name;
	ExprAST *index;

	ArrayVariableExprAST(const string variableName, ExprAST *i)
		:name(variableName), index(i){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//ArrayVariableExprAST* ErrorA(const char *str){Error(str); return 0;}
};

//记录变量表达式，如"a.p"
class RecordVariableExprAST : public ExprAST {
public:
	string name;
	ExprAST *sub;

	RecordVariableExprAST(const string variableName, ExprAST *s)
		:name(variableName), sub(s){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//RecordVariableExprAST* ErrorR(const char *str){Error(str); return 0;}	
};

//一元表达式如"!"、“-”
class UnaryExprAST : public ExprAST {
public:
	UnaryOpKind op;
	ExprAST* expr;

	UnaryExprAST(UnaryOpKind o, ExprAST *e):op(o), expr(e){}
	UnaryExprAST(const UnaryExprAST &f):op(f.op), expr(f.expr){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// UnaryExprAST* ErrorU(const char *str){Error(str); return 0;}
};


//二元表达式如“+”、“-”、“*”、“/”、“=”等
class BinaryExprAST : public ExprAST {
public:
	BinaryOpKind op;
	ExprAST *LExpr, *RExpr;

	BinaryExprAST(BinaryOpKind binaryOp, ExprAST *left, ExprAST *right)
		:op(binaryOp), LExpr(left), RExpr(right){}
	BinaryExprAST(const BinaryExprAST &f)
		:op(f.op), LExpr(f.LExpr), RExpr(f.RExpr){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// BinaryExprAST* ErrorB(const char *str){Error(str); return 0;}
};

//调用函数表达式，包含函数名称和参数
class CallExprAST : public ExprAST {
public:
	string callee;
	std::vector<ExprAST*> args;
	bool isSystemCall;

	CallExprAST(const string name, std::vector<ExprAST*> vec, bool is)
		:callee(name), args(vec), isSystemCall(is){}
	CallExprAST(const CallExprAST &f): callee(f.callee), args(f.args), isSystemCall(f.isSystemCall){}
	Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
	// CallExprAST* ErrorC(const char *str){Error(str); return 0;}
};

//if else表达式
class IfExprAST : public ExprAST {
public:
	ExprAST *ifCond;
	std::vector<ExprAST*> thenComponent, elseComponent;

	IfExprAST(ExprAST *cond, std::vector<ExprAST*> thenCo, std::vector<ExprAST*> elseCo)
		:ifCond(cond), thenComponent(thenCo), elseComponent(elseCo){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//IfExprAST* ErrorI(const char *str){Error(str); return 0;}
};

//for表达式
class ForExprAST : public ExprAST {
public:
	string varName;
	ExprAST *start, *end;
	std::vector<ExprAST*> body;
	bool increaseDirection;

	ForExprAST(const string name, ExprAST *st, ExprAST *en, std::vector<ExprAST*> bo, bool inc)
		:varName(name), start(st), end(en), body(bo), increaseDirection(inc){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//ForExprAST* ErrorF(const char *str){Error(str); return 0;}
};

//while表达式
class WhileExprAST : public ExprAST {
public:
	ExprAST *whileCond;
	std::vector<ExprAST*> body;

	WhileExprAST(ExprAST *cond, std::vector<ExprAST*> bo)
		:whileCond(cond), body(bo){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//WhileExprAST* ErrorW(const char *str){Error(str); return 0;}
};

//repeat until表达式
class RepeatExprAST : public ExprAST {
public:
	ExprAST* untilCond;
	std::vector<ExprAST*> body;

	RepeatExprAST(ExprAST *cond, std::vector<ExprAST*> bo)
		:untilCond(cond), body(bo){}
	//Value* Codegen(CodeGenContext& context) override;
	//void print(int n) override;
	//RepeatExprAST* ErrorR(const char *str){Error(str); return 0;}	
};

//函数，包含原型和函数体表达式
typedef enum { type_function, type_procedure} functionType;
class FunctionAST : public Node{
public:
	string name;
	std::vector<VariableDeclAST*> headerDecl;
	BasicTypeAST* returnType;
	std::vector<ConstAST*> consts;
	std::vector<SelfdefineTypeAST*> selfdefineType;
	std::vector<VariableDeclAST*> bodyDecl;
	std::vector<FunctionAST*> functions;
	std::vector<ExprAST*> body;

    functionType type;//区分是procedure还是function
    bool isFunction() { return type == type_function; }
    bool isProcedure() { return type == type_procedure; }

	FunctionAST(string name, vector<VariableDeclAST *>args, BasicTypeAST *returnType, 
				vector<ConstAST*> con, vector<SelfdefineTypeAST*> de, 
				vector<VariableDeclAST*> funcDecl, vector<FunctionAST *> subFun, 
				vector<ExprAST*> funcBody, functionType t)
			:name(name), headerDecl(args), returnType(returnType), 
			consts(con), selfdefineType(de), bodyDecl(funcDecl), functions(subFun), 
			body(funcBody), type(t){}
	FunctionAST(const FunctionAST &f)
		:name(f.name), headerDecl(f.headerDecl), returnType(f.returnType),consts(f.consts), selfdefineType(f.selfdefineType), bodyDecl(f.bodyDecl), functions(f.functions), body(f.body), type(f.type){}
    Value* Codegen(CodeGenContext &astcontext) override;
	void print(int n) override;
    // FunctionAST* ErrorF(const char * str) {Error(str); return 0;}
};

class CodeGenContext{
public:
    CodeGenContext *parent;
    map<string, Type*> typeTable;
    map<string, FunctionAST*> functionTable;
    map<string, Value*> locals;
    FunctionAST *currentFunc;

    CodeGenContext(CodeGenContext *parent=NULL):parent(parent){
        if(parent != NULL){
            currentFunc = parent->currentFunc;
        }else{
            currentFunc = NULL;
        }
    }
    // Type* getType(string name);
    // FunctionAST* getFunction(string name);
    // Value* getVar(string name);
    // bool addFunction(string name, FunctionAST *FunctionAST);
    // bool addVar(string name, Value *var);
    // bool addType(string name, Type *type);
};

#endif
