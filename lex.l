%{
#include "libs/structures.h"
#include "parser.tab.h"
// #include "libs/ast.c"
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

"int"                       {printf("%s\n", yytext); return TOK_INT;}
"float"                     {printf("%s\n", yytext); return TOK_FLOAT;}
"char"                      {printf("%s\n", yytext); return TOK_CHAR;}
"string"                    {printf("%s\n", yytext); return TOK_STRING;}
"print"                     {printf("%s\n", yytext); return TOK_PRINT;}
"if"                        {printf("%s\n", yytext); return TOK_IF;}
"else"                      {printf("%s\n", yytext); return TOK_ELSE;}
"while"                     {printf("%s\n", yytext); return TOK_WHILE;}
"proc"                      {printf("%s\n", yytext); return TOK_PROC;}
"main"                      {printf("%s\n", yytext); return TOK_MAIN;}
"return"                    {printf("%s\n", yytext); return TOK_RET;}
"class"                     {printf("%s\n", yytext); return TOK_CLASS;}
"input"                     {printf("%s\n", yytext); return TOK_INPUT;}
"import"                    {printf("%s\n", yytext); return TOK_IMPORT;}
":"                         {printf("%s\n", yytext); return TOK_COLON;}
"::"                        {printf("%s\n", yytext); return TOK_DOUBLE_COLON;}
"<"                         {printf("%s\n", yytext); return TOK_LT;}
">"                         {printf("%s\n", yytext); return TOK_GT;}
"<="                        {printf("%s\n", yytext); return TOK_LET;}
">="                        {printf("%s\n", yytext); return TOK_GET;}
"=="                        {printf("%s\n", yytext); return TOK_EQ;}
"!="                        {printf("%s\n", yytext); return TOK_DIF;}
"="                         {printf("%s\n", yytext); return TOK_ATTR;}
"&&"                        {printf("%s\n", yytext); return TOK_LOG_AND;}
"||"                        {printf("%s\n", yytext); return TOK_LOG_OR;}
"not"                       {printf("%s\n", yytext); return TOK_LOG_NOT;}
"++"                        {printf("%s\n", yytext); return TOK_INCR;}
{ID}                        {printf("%s\n", yytext); strcpy(yylval.id, yytext); return TOK_ID;}
{INT_NUMBER}                {printf("%s\n", yytext); yylval.Var_Value.type = INT; yylval.Var_Value.value.intValue = atoi(yytext); return TOK_INT_NUMBER;}
{FLOAT_NUMBER}              {printf("%s\n", yytext); yylval.Var_Value.type = FLOAT; yylval.Var_Value.value.floatValue = atof(yytext); return TOK_FLOAT_NUMBER;}
{STRING_VALUE}              {printf("%s\n", yytext); yylval.Var_Value.type = STRING; strcpy(yylval.Var_Value.value.strValue, yytext); return TOK_STRING_VALUE;}
{CHAR_VALUE}                {printf("%s\n", yytext); yylval.Var_Value.type = CHAR; char str[3]; strcpy(str, yytext); yylval.Var_Value.value.charValue = str[1]; return TOK_CHAR_VALUE;}
";"                         {printf("%s\n", yytext); return TOK_SEMI;}
[ \t\v\n\f\r]+              {}
\/\/.*                      {}
\/\*(\*(\/)|[^*])*\*\/      {}
.                           {return yytext[0];}

%%
