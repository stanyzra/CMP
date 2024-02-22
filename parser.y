%{
#include <stdio.h>
#include "uthash.h"
#include <string.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);
%}

%token INT FLOAT CHAR STRING ID INT_NUMBER FLOAT_NUMBER STRING_VALUE CHAR_VALUE PLUS MINUS TIMES DIVIDE LPAREN RPAREN SEMI EQUALS

%left '+' '-'
%left '*' '/'
%nonassoc MINUS_SIGNAL
%%

program:
    | program statement
    ;

statement:
      expr SEMI
    | type ID '=' expr SEMI
    ;

expr:
      expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | '(' expr ')'
    | '-' expr %prec MINUS_SIGNAL
    ;

value:
      INT_NUMBER
    | FLOAT_NUMBER
    | ID

operator:
      PLUS
    | MINUS
    | TIMES
    | DIVIDE

type:
      INT
    | FLOAT   
    | STRING   
    | CHAR  

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
