%{
    #include <stdio.h>
    #include <stdlib.h>
%}

KEYWORD "int"|"float"|"char"|"if"|"else"|"while"|"return"|"for"
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
NUMBER [0-9]*
FLOAT [0-9]*[.][0-9]*
OPERATORS [-+()=/*<>{}]
COMMENT	"//".*\n
MULTICOMMENT "/*".*"*/"
%%

{KEYWORD}      {printf("keyword:%s\n",yytext);}
{IDENTIFIER}   {printf("identifier:%s\n",yytext);}
{NUMBER}       {printf("numbers:%s\n",yytext);}
{FLOAT}        {printf("float:%s\n",yytext);}
{OPERATORS}    {printf("operators:%s\n",yytext);}
{COMMENT}      {printf("comment:%s\n",yytext);}
{MULTICOMMENT} {printf("multicomment:%s\n",yytext);}
[ \t\n]+     ;
.            ;
%%

int main(int argc,char *argv[]){


        yyin = fopen(argv[1],"r");
        
    yylex();
        fclose(yyin);
    return 0;
}

int yywrap(){
	return 1;
}
