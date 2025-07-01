%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void yyerror(const char *s);

%}

%option noyywrap

DIGIT		[0-9]
LETTER		[a-zA-Z_]
ID		{LETTER}({LETTER}|{DIGIT})*
INT_LIT		{DIGIT}+
FLOAT_LIT	{DIGIT}+"."{DIGIT}*|{DIGIT}*"."{DIGIT}+
STR_LIT		\"([^"\\]|\\.)*\"
WS		[ \t\n]
COMMENT		"//".*|"/*"([^*]|"*"[^/])*"*/"

%%
"int"		{ return INT; }
"char"		{ return CHAR; }
"float"		{ return FLOAT; }
"if"		{ return IF; }
"else"		{ return ELSE; }
"while"		{ return WHILE; }
"return"	{ return RETURN; }
"void"		{ return VOID; }

"=="		{ return EQ; }
"!="		{ return NE; }

"{"		{ return '{'; }
"}"		{ return '}'; }
"("		{ return '('; }
")"		{ return ')'; }
";"		{ return ';'; }
","		{ return ','; }
"="		{ return '='; }
"<"		{ return '<'; }
">"		{ return '>'; }
"+"		{ return '+'; }
"-"		{ return '-'; }
"*"		{ return '*'; }
"/"		{ return '/'; }

{FLOAT_LIT} { yylval.float_val = atof(yytext); return FLOAT_LIT; }
{INT_LIT}	{ yylval.int_val = atoi(yytext); return INT_LIT; }
{STR_LIT}	{ yylval.str_val = strdup(yytext); return STR_LIT; }
{ID}		{ yylval.id = strdup(yytext); return ID; }
{WS}		{  }
{COMMENT}	{  }

.		{yyerror("Invalid character"); }

%%
