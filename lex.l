%{
#include <stdio.h>
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap

INT_NUMBER      [+-]?[0-9]+
FLOAT_NUMBER    [+-]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][+-]?[0-9]+)?
STRING_VALUE    \"([^\"\\\n]*(\\.[^\"\\\n]*)*)\"
CHAR_VALUE      '[^']'
ID              [a-zA-Z_][a-zA-Z0-9_]*

%%

"int"               { printf("TOKEN:INT\n"); return INT;}
"float"             { printf("TOKEN:FLOAT\n"); return FLOAT;}
"char"              { printf("TOKEN:CHAR\n"); return CHAR;}
"string"            { printf("TOKEN:STRING\n"); return STRING;}
{ID}                { printf("TOKEN:ID\n"); strcpy(yylval.id, yytext); return ID;}
{INT_NUMBER}        { printf("TOKEN:INT_NUMBER\n"); yylval.int_number = atoi(yytext); return INT_NUMBER;}
{FLOAT_NUMBER}      { printf("TOKEN:FLOAT_NUMBER\n"); yylval.float_number = atof(yytext); return FLOAT_NUMBER;}
{STRING_VALUE}      { printf("TOKEN:STRING_VALUE\n"); strcpy(yylval.string_value, yytext); return STRING_VALUE;}
{CHAR_VALUE}        { printf("TOKEN:CHAR_VALUE\n"); strcpy(yylval.char_value, yytext); return CHAR_VALUE;}
";"                 { printf("TOKEN:SEMI\n"); return SEMI;}
[ \t]+              { /* Ignorar espaços em branco */ }
.                   { printf("TOKEN:MISMATCH\n"); return -1;}

%%

/* int yywrap(){}

int main(int argc, char **argv)
{
    yylex();
    return 0;
} */
