%{
#include <stdio.h>
#include <stdlib.h>
#include "libs/utils.c"

int yylex(void);
int yyparse(void);
void yyerror(const char *s);

struct symbol_table *variables = NULL; /* important! initialize to NULL */

%}

%union {
    struct Var_Value var_val;
    char id[64];
}

%token <id> TOK_ID
%token <var_val> TOK_INT_NUMBER
%token <var_val> TOK_FLOAT_NUMBER
%token <var_val> TOK_STRING_VALUE
%token <var_val> TOK_CHAR_VALUE

%type <var_val> expr 
%token TOK_INT TOK_FLOAT TOK_CHAR TOK_STRING TOK_SEMI TOK_PRINT 
%token TOK_WHILE TOK_PROC TOK_RET TOK_CLASS TOK_COLON TOK_LT TOK_GT
%token TOK_LET TOK_GET TOK_EQ TOK_DIF TOK_ATTR TOK_IF TOK_ELSE TOK_MAIN
%token TOK_LOG_AND TOK_LOG_OR TOK_LOG_NOT TOK_DOUBLE_COLON TOK_INCR
%token TOK_IMPORT
%token '(' ')' '{' '}' '[' ']' ':' '+' '-' '*' '/'

%left '+' '-'
%left '*' '/'
%left TOK_LOG_OR
%left TOK_LOG_AND
%right TOK_LOG_NOT
%nonassoc MINUS_SIGNAL

%%

program:
      imports classes procedures main
    ;

import_command:
      TOK_IMPORT expr
    ;

imports:
    | imports import_command

arguments:
    | TOK_ID TOK_DOUBLE_COLON primitive_type
    | TOK_ID TOK_DOUBLE_COLON class_type
    | TOK_ID TOK_DOUBLE_COLON primitive_type ',' arguments
    | TOK_ID TOK_DOUBLE_COLON class_type ',' arguments
    ;

procedure:
    TOK_PROC TOK_ID '(' arguments ')' TOK_DOUBLE_COLON primitive_type '{' statement TOK_RET procedure_return TOK_SEMI '}'
    ;

procedure_return:
      expr
    | boolean_expr
    ;

procedures:
    | procedure procedures
    ;

main:
    TOK_MAIN '(' ')' TOK_DOUBLE_COLON TOK_INT '{' statement '}'
    ;

class_declaration:
      TOK_CLASS TOK_ID '{' declaration statement '}'
    ;

classes:
    | class_declaration classes
    ;

statement:
    | declaration statement
    | becomes statement
    | TOK_PRINT '(' expr ')' TOK_SEMI
    | if_command statement
    | while_command statement
    ;

class_type:
    TOK_ID
    ;

declaration:
      TOK_ID TOK_DOUBLE_COLON primitive_type becomes
    | TOK_ID TOK_DOUBLE_COLON class_type becomes
    | increment
    ;

becomes:
      TOK_ATTR expr TOK_SEMI
    | TOK_ATTR '[' array_elements ']' TOK_SEMI
    | TOK_ATTR TOK_ID '(' ')' TOK_SEMI
    ;

array_elements:
    | expr
    | expr ',' array_elements
    ;

expr:
      value
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    ;

value:
      TOK_INT_NUMBER
    | TOK_FLOAT_NUMBER
    | TOK_STRING_VALUE
    | TOK_CHAR_VALUE
    | TOK_ID
    | '(' expr ')'
    | '-' expr %prec MINUS_SIGNAL
    ;

primitive_type:
      TOK_INT
    | TOK_FLOAT   
    | TOK_STRING   
    | TOK_CHAR  
    ;

if_command:
      TOK_IF '(' boolean_expr ')' '{' statement '}' else_options
    ;

else_options:
    | TOK_ELSE '{' statement '}' else_options
    | TOK_ELSE if_command
    ;

boolean_comparission:
      expr logical_operator expr
    ;

boolean_expr:
    | boolean_comparission
    | TOK_LOG_NOT boolean_expr
    | boolean_expr TOK_LOG_AND boolean_expr
    | boolean_expr TOK_LOG_OR boolean_expr
    | '(' boolean_expr ')'
    ;

logical_operator:
      TOK_GT 
    | TOK_GET
    | TOK_LT 
    | TOK_LET
    | TOK_EQ 
    | TOK_DIF
    ;

while_command:
    TOK_WHILE '(' boolean_expr ')' '{' statement '}'
    ;

increment:
    TOK_ID TOK_INCR TOK_SEMI
    ;

%%

void yyerror(const char *s) {
	extern int yylineno;    
	extern char * yytext;   
    printf("Error: %s for symbol '%s' on line %d", s, yytext, yylineno);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
