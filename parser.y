%{
#include <stdio.h>
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
    | ID EQUALS expr SEMI
    ;

expr:
      expr '+' expr
      expr '-' expr
      expr '*' expr
      expr '/' expr
      '(' expr ')'
      '-' expr %prec MINUS_SIGNAL
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

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
