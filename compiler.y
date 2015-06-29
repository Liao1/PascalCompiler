%{
	#include<stdio.h>
	#include<string.h>
	#include "AST.h"
%}
%union{
	int intValue;
	char charValue;
	Node *treeNode;
	double floatValue;
	char strValue[256];
	vector<ExprAST *> exprlist;
	vector<ConstAST *> constlist;
	vector<SelfdefineTypeAST *> typelist;
	vector<VariableDeclAST *> varlist;
	vector<FunctionAST *> funclist;
}
%type <treeNode> expression expr term factor const_value 
%type <treeNode> stmt non_label_stmt assign_stmt proc_stmt if_stmt repeat_stmt while_stmt for_stmt goto_stmt compound_stmt case_stmt case_expr_list case_expr else_clause
%type <treeNode> routine routine_head routine_body 
%type <treeNode> const_expr_list type_decl_list type_definition type_decl simple_type_decl sys_type name_list
%type <treeNode> array_type_decl record_type_decl field_decl_list field_decl 
%type <treeNode> var_decl_list var_decl sub_routine function_decl function_head procedure_decl procedure_head
%type <treeNode> parameters para_decl_list para_type_list var_para_list val_para_list
%type <intValue> direction 
%type <exprlist> args_list expression_list stmt_list
%type <constlist> const_part
%type <typelist> type_part
%type <varlist> var_part
%type <funclist> routine_part
%token GE GT LE LT EQUAL UNEQUAL
%token PLUS MINUS OR MUL DIV MOD AND NOT ASSIGN
%token ID LP RP DOT COMMA LB RB SEMI COLON
%token  SYS_TYPE
%token <intValue>INTEGER CHAR BOOLEAN
%token <floatValue>REAL
%token <strValue> STRING NAME SYS_FUNCT SYS_PROC
%token TYPE_INTEGER TYPE_REAL TYPE_CHAR TYPE_BOOLEAN TYPE_STRING
%token READ IF THEN ELSE REPEAT UNTIL WHILE DO FOR TO DOWNTO
%token CASE OF GOTO
%token BEGINNING END PROGRAM CONST TYPE ARRAY RECORD VAR
%token FUNCTION PROCEDURE
%%
/*program derivation*/
program:
	program_head routine DOT	{ root =  }
	;
program_head:
	PROGRAM NAME SEMI	
	;
/*routine derivation*/
routine:
	routine_head routine_body	{ $$ = new FunctionAST(NULL, $1, $2); }
	;
routine_head:
	label_part const_part type_part	var_part routine_part	{ $$ = new FunctionHeadAST($2, $3, $4, $5); }
	;
sub_routine:
	routine_head routine_body	{ $$ = new FunctionAST($1, $2); }
	;
/* label part*/
label_part:	
	;
/* const part*/
const_part:
	CONST const_expr_list	{ $$ = $2; }
	|	{ $$ = NULL; }
	;
const_expr_list:
	const_expr_list NAME EQUAL const_value SEMI	{
		$1.push_back(new ConstAST($2, ))
		$$ = ConnectNodes($1, CreateConst($2, $4)); }
	| NAME EQUAL const_value SEMI	{ $$ = CreateConst($1, $3); }
	;
/* type part*/
type_part:
	TYPE type_decl_list	{ $$ = $2; }	
	|	{ $$ = NULL; }
	;
type_decl_list:
	type_decl_list type_definition	{ $$ = ConnectNodes($1, $2); }
	| type_definition	{ $$ = $1; }
	;
type_definition:
	NAME EQUAL type_decl SEMI	{ 
		$$ = CreateTypeDef($1, $3); 
	}
	;
type_decl:
	simple_type_decl	{ $$ = $1; }
	| array_type_decl	{ $$ = $1; }
	| record_type_decl	{ $$ = $1; }
	;
simple_type_decl:
	sys_type	{ $$ = $1; }
	| NAME		{ $$ = CreateSelfDefineType($1); }		
	| LP name_list RP	{ $$ = CreateEnumType($2, NULL); }
	| const_value DOT DOT const_value	{ $$ = CreateEnumType($1, $4); }
	| MINUS const_value DOT DOT const_value		{ 
		TreeNode *p;
		p = CreateOpExp(neg_kind); p->children[0] = $2;
		$$ = CreateEnumType(p, $5); 
	}
	| MINUS const_value DOT DOT MINUS const_value	{ 
		TreeNode *p, *q;
		p = CreateOpExp(neg_kind); p->children[0] = $2;
		q = CreateOpExp(neg_kind); q->children[0] = $6;
		$$ = CreateEnumType(p, q); 
	}
	| NAME DOT DOT NAME	{ $$ = CreateEnumType(CreateIdExp($1), CreateIdExp($4)); }
	;
sys_type:
	TYPE_INTEGER	{ $$ = CreateSimpleType(integer_type); }
	| TYPE_REAL	{ $$ = CreateSimpleType(real_type); }
	| TYPE_BOOLEAN	{ $$ = CreateSimpleType(boolean_type); }
	| TYPE_CHAR	{ $$ = CreateSimpleType(char_type); }
	| TYPE_STRING	{ $$ = CreateSimpleType(string_type); }
	;
array_type_decl:
	ARRAY LB simple_type_decl RB OF type_decl	{ $$ = CreateArrayType($3, $6); }
	;
record_type_decl:
	RECORD field_decl_list END		{ $$ = CreateRecordType($2); }
	;
field_decl_list:
	field_decl_list field_decl		{ $$ = ConnectNodes($1, $2); }
	| field_decl		{ $$ = $1; }
	;
field_decl:
	name_list COLON type_decl SEMI	{ $$ = CreateVar($1, $3); }
	;
name_list:
	name_list COMMA NAME	{ $$ = ConnectNodes($1, CreateIdExp($3)); }
	| NAME		{ $$ = CreateIdExp($1); }
	;
/* var part*/
var_part:
	VAR var_decl_list	{ $$ = $2; }
	|	{ $$ = NULL; }
	;
var_decl_list:
	var_decl_list var_decl	{ $$ = ConnectNodes($1, $2); }	
	| var_decl		{ $$ = $1; }
	;
var_decl:
	name_list COLON type_decl SEMI		{ $$ = CreateVar($1, $3); }
	;
/* routine part*/
routine_part: 
	routine_part function_decl	{ $$ = ConnectNodes($1, $2); }
	| routine_part procedure_decl	{ $$ = ConnectNodes($1, $2); }
	| function_decl		{ $$ = $1; }
	| procedure_decl	{ $$ = $1; }
	| 	{ $$ = NULL; }
	;
function_decl:
	function_head SEMI sub_routine SEMI	{ $$ = $1; $1->children[1] = $3; }
	;
function_head:
	FUNCTION NAME parameters COLON simple_type_decl		{ $$ = CreateFuncHead($2, $3, $5); }
	;
procedure_decl:
	procedure_head SEMI sub_routine SEMI	{ $$ = $1; $1->children[1] = $3; }
	;
procedure_head:
	PROCEDURE NAME parameters	{ $$ = CreateProcHead($2, $3); }
	;
parameters:
	LP para_decl_list RP	{ $$ = $2; }
	|	{ $$ = NULL; }
	;
para_decl_list:
	para_decl_list SEMI para_type_list	{ $$ = ConnectNodes($1, $3); }
	| para_type_list	{ $$ = $1; }
	;
para_type_list:
	var_para_list COLON simple_type_decl	{ $$ = CreateParam($1, $3, param_var_kind); }
	| val_para_list COLON simple_type_decl	{ $$ = CreateParam($1, $3, param_val_kind); }
	;
var_para_list:
	VAR name_list	{ $$ = $2; }
	;
val_para_list:
	name_list	{ $$ = $1; }
	;
/* routine_body*/
routine_body:
	compound_stmt	{ $$ = $1; }
	;
/*stmt derivation*/
stmt_list:
	stmt_list stmt SEMI		{ 
		for (int i=0; i<$2.size(); i++)
			$1.push_back($2[i]);
		$$ = $1; 
	}
	|		{ 
		vector<ExprAST *> tmp;
		$$ = tmp;
	}
	;
stmt:
	INTEGER COLON non_label_stmt	{ $$ = NULL; }
	| non_label_stmt	{ $$.push_back($1); }
	;
non_label_stmt:
	assign_stmt		{ $$ = $1; }
	| proc_stmt		{ $$ = $1; }
	| if_stmt		{ $$ = $1; }
	| repeat_stmt		{ $$ = $1; }
	| while_stmt		{ $$ = $1; }
	| for_stmt		{ $$ = $1; }
	| case_stmt		{ $$ = NULL; }
	| goto_stmt		{ $$ = NULL; }
	| compound_stmt		{ $$ = $1; }
	;
compound_stmt:
	BEGINNING stmt_list END		{ $$ = new CompoundExprAST($2); }
	;
assign_stmt:
	NAME ASSIGN expression 		{ 
		$$ = new BinaryExprAST(
			assignment_stmt, 
			VariableExprAST($1),
			$3
			); 
	}
	| NAME LB expression RB ASSIGN expression	{
		$$ = new BinaryExprAST(
			assignment_stmt,
			ArrayVariableExprAST($1, $3),
			$6
			);
	}
	| NAME DOT NAME ASSIGN expression 	{
		$$ = new BinaryExprAST(
			assignment_stmt,
			RecordVariableExprAST($1, VariableExprAST($3)),
			$5
			);
	}
	;
proc_stmt:
	NAME 	{ 
		vector<ExprAST *> tmp;
		$$ = new CallExprAST($1, tmp);
	}
	| NAME LP args_list RP	{
		$$ = new CallExprAST($1, $3);
	}
	| SYS_PROC	{ $$ = NULL; }
	| SYS_PROC LP expression_list RP	{
		$$ = new CallExprAST($1, $3);
		// $$->system = 1;
	}		
	| READ LP factor RP {
		$$ = new CallExprAST("read", $3);
	}
	;
if_stmt:
	IF expression THEN stmt else_clause		{ 
		$$ = new IfExprAST($2, $4, $5);
	}
	;
else_clause:
	ELSE stmt	{ $$ = $2; }
	|		{ $$ = NULL; }
	;
repeat_stmt:
	REPEAT stmt_list UNTIL expression	{ 
		$$ = new RepeatExprAST($4, $2);
	}
	;
while_stmt:
	WHILE expression DO stmt	{
		$$ = new WhileExprAST($2, $4);
	}
	;
for_stmt:
	FOR NAME ASSIGN expression direction expression DO stmt		{
		$$ = NULL;
	}
	;
direction:
	TO		{ $$ = 1; }
	| DOWNTO	{ $$ = 0; }
	;
case_stmt:
	CASE expression OF case_expr_list END	{
		$$ = NULL;
	}
	;
case_expr_list:
	case_expr_list case_expr	{ $$ = NULL; }
	| case_expr		{ $$ = NULL; }
	;
case_expr:
	const_value COLON stmt SEMI	{ 
		$$ = NULL
	}
	| NAME COLON stmt SEMI		{
		$$ = NULL
	}
	;
goto_stmt:
	GOTO INTEGER		{
		$$ = NULL;
	}
	;
/*expression derivation*/
expression_list:
	expression_list COMMA expression 	{ 
		$1.push_back($3); 
		$$=$1;
	}
	| expression	{ $$.push_back($1); }
	;
expression:
	expression GE expr		{
		$$ = new BinaryExprAST(ge_kind, $1, $3);
	}	
	| expression GT expr 		{
		$$ = new BinaryExprAST(gt_kind, $1, $3);
	}
	| expression LE expr 		{
		$$ = new BinaryExprAST(le_kind, $1, $3);
	}
	| expression LT expr 		{
		$$ = new BinaryExprAST(lt_kind, $1, $3);
	}
	| expression EQUAL expr		{
		$$ = new BinaryExprAST(eq_kind, $1, $3);
	}
	| expression UNEQUAL expr	{
		$$ = new BinaryExprAST(ueq_kind, $1, $3);
	}
	| expr		{ $$ = $1; }
	;
expr:
	expr PLUS term		{
		$$ = new BinaryExprAST(plus_kind, $1, $3);
	}
	| expr MINUS term	{
		$$ = new BinaryExprAST(minus_kind, $1, $3);
	}
	| expr OR term		{
		$$ = new BinaryExprAST(or_kind, $1, $3);
	}
	| term		{ $$ = $1; }
	;
term:
	term MUL factor		{ 
		$$ = new BinaryExprAST(mul_kind, $1, $3);
	}
	| term DIV factor	{ 
		$$ = new BinaryExprAST(div_kind, $1, $3);
	}
	| term MOD factor	{ 
		$$ = new BinaryExprAST(mod_kind, $1, $3);
	}
	| term AND factor	{ 
		$$ = new BinaryExprAST(and_kind, $1, $3);
	}
	| factor	{ $$ = $1; }
	;
factor:
	NAME		{ 
		//$$ = new VariableExprAST($1); 
	}
	| NAME LP args_list RP		{ 
		//$$ = new CallExprAST($1, $3); 
	}
	| SYS_FUNCT	{ 
		//$$ = NULL; 
	}
	| SYS_FUNCT LP args_list RP	{
		//$$ = new CallExprAST($1, $3);
		//$$->system = 1;
	}
	| const_value	{ 
		//$$ = $1; 
	}
	| NOT factor	{
		//$$ = new CreateOpExp(not_kind, $2);
	}
	| MINUS factor	{
		//$$ = new UnaryExprAST(neg_kind, $2);
	}
	| LP expression RP { 
		//$$ = $2; 
	}
	| NAME LB expression RB		{ 
		//$$ = new ArrayVariableExprAST($1, $3); 
	}
	| NAME DOT NAME	{ 
		//$$ = new VariableExprAST($1, new VariableExprAST($3); 
	}
	;
args_list:
	args_list COMMA expression	{ 
		//$1.push_back($3); 
		//$$=$1; 
	}
	| expression 	{ 
		//$$.push_back($1);
	}
	;
const_value:
	INTEGER		{ 
		$$ = new NumberExprAST($1);
	}
	| REAL		{
		$$ = new RealExprAST($1);
	}
	| CHAR		{
		$$ = new CharExprAST($1);
	}
	| STRING	{
		char strVal[256];
		strCatch(strVal, $1);
		$$ = new StringExprAST($1);
	}
	| BOOLEAN	{
		$$ = new BoolExprAST($1);
	}
	;
%%
int main(int argc, char *argv[]){
	yyin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	lineNo = 1;
	/* parsing tree */
	yyparse();
	/* end */
	fclose(yyin);
	fclose(fout);
	return 0;
}

void yyerror(char *s){
	fprintf(stderr, "%s\n", s);
}

