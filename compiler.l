%{
	#include<stdio.h>
	#include "Global.h"
	#include "compiler.tab.c"
%}
%%
">="	return GE;
">"	return GT;
"<="	return LE;
"<"	return LT;
"="	return EQUAL;
"<>"	return UNEQUAL;
"+"	return PLUS;
"-"	return MINUS;
"*"	return MUL;
"/"	return DIV;
"."	return DOT;
","	return COMMA;
"["	return LB;
"]"	return RB;
"("	return LP;
")"	return RP;
":="	return ASSIGN;
";"	return SEMI;
":"	return COLON;
[a-zA-Z][a-zA-Z0-9_]*	{
		int i;
		/* reserved word */
		if ((i=resWord(yytext))!=0)
			return i;
		/* system functions */
		if ((i=sys_function(yytext))!=0){
			strcpy(yylval.strValue, yytext);
			return SYS_FUNCT;
		}
		/* const boolean value */
		if (!strcmp(yytext, "false")){
			yylval.intValue = 0;
			return BOOLEAN;
		}
		if (!strcmp(yytext, "true")){
			yylval.intValue = 1;
			return BOOLEAN;
		}
		/* const integer value */
		if (!strcmp(yytext, "maxint")){
			yylval.intValue = 32767;
			return INTEGER;
		}
		/* system procedure */
		if ((i=sys_proc(yytext))!=0){
			strcpy(yylval.strValue, yytext);
			return SYS_PROC;
		}
		/* system types */
		if ((i=sys_type(yytext))!=0)
			return i;
		strcpy(yylval.strValue, yytext);
		return NAME;
	}
[0-9]+	{
			yylval.intValue = atoi(yytext);
			return INTEGER;
		}
[0-9]+\.[0-9]+	{
			yylval.floatValue = atof(yytext);
			return REAL;
		}
\'.\'		{
			yylval.charValue = yytext[1];
			return CHAR;
		}
\'[^'\n]*\'	{
			strcpy(yylval.strValue, yytext);
			return STRING;
		}
\n		{ 
			lineNo++; 
		}
.	{}
%%
int yywrap(void){
	return 1;
}

int sys_function(char *s){
	if (!strcmp(s, "abs")) return 1;
	if (!strcmp(s, "chr")) return 1;
	if (!strcmp(s, "odd")) return 1;
	if (!strcmp(s, "ord")) return 1;
	if (!strcmp(s, "pred")) return 1;
	if (!strcmp(s, "sqr")) return 1;
	if (!strcmp(s, "sqrt")) return 1;
	if (!strcmp(s, "succ")) return 1;
	return 0;
}
int resWord(char *s){
	if (!strcmp(s, "program")) return PROGRAM;
	if (!strcmp(s, "const")) return CONST;
	if (!strcmp(s, "type")) return TYPE;
	if (!strcmp(s, "array")) return ARRAY;
	if (!strcmp(s, "record")) return RECORD;
	if (!strcmp(s, "var")) return VAR;
	if (!strcmp(s, "function")) return FUNCTION;
	if (!strcmp(s, "procedure")) return PROCEDURE;
	if (!strcmp(s, "or")) return OR;
	if (!strcmp(s, "and")) return AND;
	if (!strcmp(s, "mod")) return MOD;
	if (!strcmp(s, "not")) return NOT;
	if (!strcmp(s, "read")) return READ;
	if (!strcmp(s, "if")) return IF;
	if (!strcmp(s, "then")) return THEN;
	if (!strcmp(s, "else")) return ELSE;
	if (!strcmp(s, "repeat")) return REPEAT;
	if (!strcmp(s, "until")) return UNTIL;
	if (!strcmp(s, "while")) return WHILE;
	if (!strcmp(s, "do")) return DO;
	if (!strcmp(s, "for")) return FOR;
	if (!strcmp(s, "to")) return TO;
	if (!strcmp(s, "downto")) return DOWNTO;
	if (!strcmp(s, "begin")) return BEGINNING;
	if (!strcmp(s, "end")) return END;
	if (!strcmp(s, "case")) return CASE;
	if (!strcmp(s, "of")) return OF;
	if (!strcmp(s, "goto")) return GOTO;
	return 0;
}
int sys_proc(char *s){
	if (!strcmp(s, "write")) return 1;
	if (!strcmp(s, "writeln")) return 1;
	if (!strcmp(s, "readln")) return 1;
	return 0;
}
int sys_type(char *s){
	if (!strcmp(s, "boolean")) return TYPE_BOOLEAN;
	if (!strcmp(s, "char")) return TYPE_CHAR;
	if (!strcmp(s, "integer")) return TYPE_INTEGER;
	if (!strcmp(s, "real")) return TYPE_REAL;
	if (!strcmp(s, "string")) return TYPE_STRING;
	return 0;
}

