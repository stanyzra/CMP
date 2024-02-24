%{
#include "libs/structures.h"
#include "parser.tab.h"
#include <stdio.h>
%}

%option yylineno
%option noyywrap

INT_NUMBER      [+-]?[0-9]+
FLOAT_NUMBER    [+-]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][+-]?[0-9]+)?
STRING_VALUE    \"([^\"\\\n]*(\\.[^\"\\\n]*)*)\"
CHAR_VALUE      '([^'])'
ID              [a-zA-Z_][a-zA-Z0-9_]*

%%

"int"                       { return TOK_INT;}
"float"                     { return TOK_FLOAT;}
"char"                      { return TOK_CHAR;}
"string"                    { return TOK_STRING;}
"print"                     { return TOK_PRINT;}
{ID}                        { strcpy(yylval.id, yytext); return TOK_ID;}
{INT_NUMBER}                { yylval.var_val.type = INT; yylval.var_val.value.intValue = atoi(yytext); return TOK_INT_NUMBER;}
{FLOAT_NUMBER}              { yylval.var_val.type = FLOAT; yylval.var_val.value.floatValue = atof(yytext); return TOK_FLOAT_NUMBER;}
{STRING_VALUE}              { yylval.var_val.type = STRING; strcpy(yylval.var_val.value.strValue, yytext); return TOK_STRING_VALUE;}
{CHAR_VALUE}                {yylval.var_val.type = CHAR; char str[3]; strcpy(str, yytext); yylval.var_val.value.charValue = str[1]; return TOK_CHAR_VALUE;}
";"                         { return TOK_SEMI;}
[ \t]+                      {}
\/\/.*                      {}
\/\*(\*(\/)|[^*])*\*\/      {}
.                           { return yytext[0];}

%%
