%{
#include <stdio.h>
#include <stdlib.h>
#include "libs/utils.c"
#include "libs/ast.c"

int yylex(void);
int yyparse(void);
void yyerror(const char *s);

struct symbol_table *variables = NULL; /* important! initialize to NULL */

%}


%union {
    struct import_command *import_command;
    struct program *program;
    struct statement *statement;
    struct becomes *becomes;
    struct if_command *if_command;
    struct print_command *print_command;
    struct while_command *while_command;
    struct declaration *declaration;
    struct imports *imports;
    struct arguments_declaration *arguments_declaration;
    struct procedure *procedure;
    struct procedure_return *procedure_return;
    struct procedures *procedures;
    struct class_declaration *class_declaration;
    struct classes *classes;
    struct input_call *input_call;
    struct class_type *class_type;
    struct procedure_call *procedure_call;
    struct arguments *arguments;
    struct expr *expr;
    struct value *value;
    struct primitive_type *primitive_type;
    struct else_options *else_options;
    struct boolean_comparission *boolean_comparission;
    struct boolean_expr *boolean_expr;
    struct logical_operator *logical_operator;
    struct increment *increment;
    struct array_elements *array_elements;
    struct main *main;
    struct ast *ast;
    struct Var_Value Var_Value;
    char id[64];
}

%token <id> TOK_ID
%token <Var_Value> TOK_INT_NUMBER
%token <Var_Value> TOK_FLOAT_NUMBER
%token <Var_Value> TOK_STRING_VALUE
%token <Var_Value> TOK_CHAR_VALUE

%type <program> program
%type <statement> statement
%type <becomes> becomes
%type <if_command> if_command
%type <print_command> print_command
%type <while_command> while_command
%type <declaration> declaration
%type <imports> imports
%type <arguments_declaration> arguments_declaration
%type <procedure> procedure
%type <procedure_return> procedure_return
%type <procedures> procedures
%type <class_declaration> class_declaration
%type <classes> classes
%type <input_call> input_call
%type <class_type> class_type
%type <procedure_call> procedure_call
%type <arguments> arguments
%type <expr> expr
%type <value> value
%type <primitive_type> primitive_type
%type <else_options> else_options
%type <boolean_comparission> boolean_comparission
%type <boolean_expr> boolean_expr
%type <logical_operator> logical_operator
%type <increment> increment
%type <array_elements> array_elements
%type <main> main
%type <import_command> import_command
/* %type <var_val> expr */

%token TOK_INT TOK_FLOAT TOK_CHAR TOK_STRING TOK_SEMI TOK_PRINT 
%token TOK_WHILE TOK_PROC TOK_RET TOK_CLASS TOK_COLON TOK_LT TOK_GT
%token TOK_LET TOK_GET TOK_EQ TOK_DIF TOK_ATTR TOK_IF TOK_ELSE TOK_MAIN
%token TOK_LOG_AND TOK_LOG_OR TOK_LOG_NOT TOK_DOUBLE_COLON TOK_INCR
%token TOK_IMPORT TOK_INPUT
%token '(' ')' '{' '}' '[' ']' ':' '+' '-' '*' '/'

%left '+' '-'
%left '*' '/'
%left TOK_LOG_OR
%left TOK_LOG_AND
%right TOK_LOG_NOT
/* %nonassoc MINUS_SIGNAL */

%%

program:
      imports classes procedures main
      { printf("TESTE PROGRAM \n"); create_tree((struct ast *)$1) ; create_tree((struct ast *)$2); create_tree((struct ast *)$3); create_tree((struct ast *)$4); }
    ;

import_command:
      TOK_IMPORT expr
      { $$ = (struct import_command *)new_import_command($2); }
    ;

imports:
    | imports import_command 
    { $$ = (struct imports *)new_imports($1, $2); }

arguments_declaration:
    | TOK_ID TOK_DOUBLE_COLON primitive_type
    { $$ = (struct arguments_declaration *)new_arguments_declaration(0, $1, $3, NULL, NULL); }
    | TOK_ID TOK_DOUBLE_COLON class_type
    { $$ = (struct arguments_declaration *)new_arguments_declaration(1, $1, NULL, $3, NULL); }
    | TOK_ID TOK_DOUBLE_COLON primitive_type ',' arguments_declaration
    { $$ = (struct arguments_declaration *)new_arguments_declaration(2, $1, $3, NULL, $5); }
    | TOK_ID TOK_DOUBLE_COLON class_type ',' arguments_declaration
    { $$ = (struct arguments_declaration *)new_arguments_declaration(3, $1, NULL, $3, $5); }
    ;

procedure:
    TOK_PROC TOK_ID '(' arguments_declaration ')' TOK_DOUBLE_COLON primitive_type '{' statement TOK_RET procedure_return TOK_SEMI '}'
    { $$ = (struct procedure *)new_procedure($2, $4, $7, $9, $11); }
    ;

procedure_return:
      expr
      { $$ = (struct procedure_return *)new_procedure_return(0, $1, NULL); }
    | boolean_expr
      { $$ = (struct procedure_return *)new_procedure_return(0, NULL, $1); }
    ;

procedures:
    | procedure procedures
    { $$ = (struct procedures *)new_procedures($1, $2); }
    ;

main:
      TOK_MAIN '(' ')' '{' statement '}'
      {printf("TESTE MAIN \n");$$ = (struct main *)new_main($5); }
    ;

class_declaration:
      TOK_CLASS TOK_ID '{' declaration statement '}'
      { $$ = (struct class_declaration *)new_class_declaration($2, $4, $5); }
    ;

classes:
    | class_declaration classes
    { $$ = (struct classes *)new_classes($1, $2); }
    ;

statement: 
    { printf("STATMENT NULL\n"); $$ = NULL; }
    | declaration statement
    { printf("teste declaration statment\n"); $$ = (struct statement *)new_statement(0, $1, NULL, NULL, NULL, NULL); }
    | becomes statement
    { $$ = (struct statement *)new_statement(1, NULL, $1, NULL, NULL, NULL); }
    | print_command statement
    { $$ = (struct statement *)new_statement(2, NULL, NULL, $1, NULL, NULL); }
    | if_command statement
    { $$ = (struct statement *)new_statement(3, NULL, NULL, NULL, $1, NULL); }
    | while_command statement
    { $$ = (struct statement *)new_statement(4, NULL, NULL, NULL, NULL, $1); }
    ;

print_command:
      TOK_PRINT '(' expr ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(0, $3, NULL, NULL); }
    | TOK_PRINT '(' boolean_expr ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(1, NULL, $3, NULL); }
    | TOK_PRINT '(' procedure_call ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(2, NULL, NULL, $3); }
    ;

input_call:
      TOK_INPUT '(' expr ')'
    { $$ = (struct input_call *)new_input_call($3); }
    ;

class_type:
    TOK_ID
    { $$ = (struct class_type *)new_class_type($1); }
    ;

declaration:
      TOK_ID TOK_DOUBLE_COLON primitive_type becomes
    { $$ = (struct declaration *)new_declaration(0, $1, $3, $4, NULL, NULL); }
    | TOK_ID TOK_DOUBLE_COLON class_type becomes
    { $$ = (struct declaration *)new_declaration(1, $1, NULL, $4, $3, NULL); }
    | increment
    { $$ = (struct declaration *)new_declaration(2, NULL, NULL, NULL, NULL, $1); }
    ;

becomes:
      TOK_ATTR expr TOK_SEMI
    {printf("TESTE-------------\n"); $$ = (struct becomes *)new_becomes(0, $2, NULL, NULL, NULL); }
    | TOK_ATTR '[' array_elements ']' TOK_SEMI
    { $$ = (struct becomes *)new_becomes(1, NULL, $3, NULL, NULL); }
    | TOK_ATTR procedure_call TOK_SEMI
    { $$ = (struct becomes *)new_becomes(2, NULL, NULL, $2, NULL); }
    | TOK_ATTR input_call TOK_SEMI
    { printf("TESTE INPUT\n"); $$ = (struct becomes *)new_becomes(3, NULL, NULL, NULL, $2); }
    ;

procedure_call:
      TOK_ID '(' arguments ')'
    { $$ = (struct procedure_call *)new_procedure_call($1, $3); }
    ;

arguments:
    | expr
    { $$ = (struct arguments *)new_arguments(0, $1, NULL); }
    | expr ',' arguments
    { $$ = (struct arguments *)new_arguments(1, $1, $3); }

array_elements:
    | expr
    { $$ = (struct array_elements *)new_array_elements(0, $1, NULL); }
    | expr ',' array_elements
    { $$ = (struct array_elements *)new_array_elements(1, $1, $3); }
    ;

expr:
      value
    {printf("TESTE EXPR\n"); $$ = (struct expr *)new_expr(0, $1, NULL); }
    | expr '+' expr
    {printf("TESTE EXPR\n"); $$ = (struct expr *)new_expr(1, NULL, $1); }
    | expr '-' expr
    {printf("TESTE EXPR\n"); $$ = (struct expr *)new_expr(2, NULL, $1); }
    | expr '*' expr
    {printf("TESTE EXPR\n"); $$ = (struct expr *)new_expr(3, NULL, $1); }
    | expr '/' expr
    {printf("TESTE EXPR\n"); $$ = (struct expr *)new_expr(4, NULL, $1); }
    ;

value:
      TOK_INT_NUMBER
    {printf("TESTE value\n"); $$ = (struct value *)new_value(0, NULL, NULL); }
    | TOK_FLOAT_NUMBER
    {printf("TESTE value\n"); $$ = (struct value *)new_value(1, NULL, NULL); }
    | TOK_STRING_VALUE
    {printf("TESTE value\n"); $$ = (struct value *)new_value(2, NULL, NULL); }
    | TOK_CHAR_VALUE
    {printf("TESTE value\n"); $$ = (struct value *)new_value(3, NULL, NULL); }
    | TOK_ID
    {printf("TESTE value\n"); $$ = (struct value *)new_value(4, $1, NULL); }
    | '(' expr ')'
    {printf("TESTE value\n"); $$ = (struct value *)new_value(5, NULL, $2); }
    /* | '-' expr %prec MINUS_SIGNAL */
    ;

primitive_type:
      TOK_INT
    { $$ = (struct primitive_type *)new_primitive_type(0); }
    | TOK_FLOAT   
    { $$ = (struct primitive_type *)new_primitive_type(1); }
    | TOK_STRING   
    { $$ = (struct primitive_type *)new_primitive_type(2); }
    | TOK_CHAR  
    { $$ = (struct primitive_type *)new_primitive_type(3); }
    ;

if_command:
      TOK_IF '(' boolean_expr ')' '{' statement '}' else_options
    { $$ = (struct if_command *)new_if_command($3, $6, $8); }
    ;

else_options:
    | TOK_ELSE '{' statement '}' else_options
    { $$ = (struct else_options *)new_else_options(0, $3, $5, NULL); }
    | TOK_ELSE if_command
    { $$ = (struct else_options *)new_else_options(1, NULL, NULL, $2); }
    ;

boolean_comparission:
      expr logical_operator expr
    { $$ = (struct boolean_comparission *)new_boolean_comparission($1, $2); }
    ;

boolean_expr:
    | boolean_comparission
    { $$ = (struct boolean_expr *)new_boolean_expr(0, $1, NULL); }
    | TOK_LOG_NOT boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(1, NULL, $2); }
    | boolean_expr TOK_LOG_AND boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(2, NULL, $1); }
    | boolean_expr TOK_LOG_OR boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(3, NULL, $1); }
    | '(' boolean_expr ')'
    { $$ = (struct boolean_expr *)new_boolean_expr(4, NULL, $2); }
    ;

logical_operator:
      TOK_GT 
    { $$ = (struct logical_operator *)new_logical_operator(0); }
    | TOK_GET
    { $$ = (struct logical_operator *)new_logical_operator(1); }
    | TOK_LT 
    { $$ = (struct logical_operator *)new_logical_operator(2); }
    | TOK_LET
    { $$ = (struct logical_operator *)new_logical_operator(3); }
    | TOK_EQ 
    { $$ = (struct logical_operator *)new_logical_operator(4); }
    | TOK_DIF
    { $$ = (struct logical_operator *)new_logical_operator(5); }
    ;

while_command:
    TOK_WHILE '(' boolean_expr ')' '{' statement '}'
    { $$ = (struct while_command *)new_while_command($3, $6); }
    ;

increment:
    TOK_ID TOK_INCR TOK_SEMI
    { $$ = (struct increment *)new_increment($1); }
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
