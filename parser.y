%{
#include <stdio.h>
#include <stdlib.h>
#include "libs/ast.c"
// #include "libs/structures.h"

char *FILE_PATH;
int yylex(void);
int yyparse(void);
void yyerror(const char *s);

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
    { 
        // traverse_imports($1, "<imports");
        // traverse_classes($2, "<classes");
        // traverse_prodecures($3, "<procedures");
        // traverse_main($4, "<main");
    }
    ;

import_command:
      TOK_IMPORT expr
      { $$ = (struct import_command *)new_import_command($2, "<import_command"); }
    ;

imports:
    { $$ = NULL; }
    | import_command imports
    { $$ = (struct imports *)new_imports($2, $1, "<imports"); }

arguments_declaration:
    { $$ = NULL; }
    | TOK_ID TOK_DOUBLE_COLON primitive_type
    { $$ = (struct arguments_declaration *)new_arguments_declaration(0, $1, $3, NULL, NULL, "<arguments_declaration"); }
    | TOK_ID TOK_DOUBLE_COLON class_type
    { $$ = (struct arguments_declaration *)new_arguments_declaration(1, $1, NULL, $3, NULL, "<arguments_declaration"); }
    | TOK_ID TOK_DOUBLE_COLON primitive_type ',' arguments_declaration
    { $$ = (struct arguments_declaration *)new_arguments_declaration(2, $1, $3, NULL, $5, "<arguments_declaration"); }
    | TOK_ID TOK_DOUBLE_COLON class_type ',' arguments_declaration
    { $$ = (struct arguments_declaration *)new_arguments_declaration(3, $1, NULL, $3, $5, "<arguments_declaration"); }
    ;

procedure:
    TOK_PROC TOK_ID '(' arguments_declaration ')' TOK_DOUBLE_COLON primitive_type '{' statement TOK_RET procedure_return TOK_SEMI '}'
    { $$ = (struct procedure *)new_procedure($2, $4, $7, $9, $11, "<procedure"); }
    ;

procedure_return:
    expr
    { $$ = (struct procedure_return *)new_procedure_return(0, $1, NULL, "<procedure_return"); }
    | boolean_expr
    { $$ = (struct procedure_return *)new_procedure_return(0, NULL, $1, "<procedure_return"); }
    ;

procedures:
    { $$ = NULL; }
    | procedure procedures
    { $$ = (struct procedures *)new_procedures($1, $2, "<procedures"); }
    ;

main:
    { $$ = NULL; }
    | TOK_MAIN '(' ')' '{' statement '}'
    { $$ = (struct main *)new_main($5, "<main"); }
    ;

class_declaration:
      TOK_CLASS TOK_ID '{' declaration statement '}'
      { $$ = (struct class_declaration *)new_class_declaration($2, $4, $5, "<class_declaration"); }
    ;

classes:
    { $$ = NULL; }
    | class_declaration classes
    { $$ = (struct classes *)new_classes($1, $2, "<classes"); }
    ;

statement: 
    { $$ = NULL; }
    | declaration statement
    { $$ = (struct statement *)new_statement(0, $1, NULL, NULL, NULL, NULL, "<statement"); }
    | becomes statement
    { $$ = (struct statement *)new_statement(1, NULL, $1, NULL, NULL, NULL, "<statement"); }
    | print_command statement
    { $$ = (struct statement *)new_statement(2, NULL, NULL, $1, NULL, NULL, "<statement"); }
    | if_command statement
    { $$ = (struct statement *)new_statement(3, NULL, NULL, NULL, $1, NULL, "<statement"); }
    | while_command statement
    { $$ = (struct statement *)new_statement(4, NULL, NULL, NULL, NULL, $1, "<statement"); }
    ;

print_command:
      TOK_PRINT '(' expr ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(0, $3, NULL, NULL, "<print_command"); }
    | TOK_PRINT '(' boolean_expr ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(1, NULL, $3, NULL, "<print_command"); }
    | TOK_PRINT '(' procedure_call ')' TOK_SEMI
    { $$ = (struct print_command *)new_print_command(2, NULL, NULL, $3, "<print_command"); }
    ;

input_call:
      TOK_INPUT '(' expr ')'
    { $$ = (struct input_call *)new_input_call($3, "<input_call"); }
    ;

class_type:
    TOK_ID
    { $$ = (struct class_type *)new_class_type($1, "<class_type"); }
    ;

declaration:
      TOK_ID TOK_DOUBLE_COLON primitive_type becomes
    { $$ = (struct declaration *)new_declaration(0, $1, $3, $4, NULL, NULL, "<declaration"); }
    | TOK_ID TOK_DOUBLE_COLON class_type becomes
    { $$ = (struct declaration *)new_declaration(1, $1, NULL, $4, $3, NULL, "<declaration"); }
    | increment
    { $$ = (struct declaration *)new_declaration(2, NULL, NULL, NULL, NULL, $1, "<declaration"); }
    | TOK_ID becomes
    { $$ = (struct declaration *)new_declaration(3, $1, NULL, $2, NULL, NULL, "<declaration"); }
    ;

becomes:
      TOK_ATTR expr TOK_SEMI
    { $$ = (struct becomes *)new_becomes(0, $2, NULL, NULL, NULL, "<becomes"); }
    | TOK_ATTR '[' array_elements ']' TOK_SEMI
    { $$ = (struct becomes *)new_becomes(1, NULL, $3, NULL, NULL, "<becomes"); }
    | TOK_ATTR procedure_call TOK_SEMI
    { $$ = (struct becomes *)new_becomes(2, NULL, NULL, $2, NULL, "<becomes"); }
    | TOK_ATTR input_call TOK_SEMI
    { $$ = (struct becomes *)new_becomes(3, NULL, NULL, NULL, $2, "<becomes"); }
    ;

procedure_call:
      TOK_ID '(' arguments ')'
    { $$ = (struct procedure_call *)new_procedure_call($1, $3, "<procedure_call"); }
    ;

arguments:
    { $$ = NULL; }
    | expr
    { $$ = (struct arguments *)new_arguments(0, $1, NULL, "<arguments"); }
    | expr ',' arguments
    { $$ = (struct arguments *)new_arguments(1, $1, $3, "<arguments"); }

array_elements:
    { $$ = NULL; }
    | expr
    { $$ = (struct array_elements *)new_array_elements(0, $1, NULL, "<array_elements"); }
    | expr ',' array_elements
    { $$ = (struct array_elements *)new_array_elements(1, $1, $3, "<array_elements"); }
    ;

expr:
      value
    { $$ = (struct expr *)new_expr(0, $1, NULL, NULL, "<expr"); }
    | expr '+' expr
    { $$ = (struct expr *)new_expr(1, NULL, $1, $3, "<expr"); }
    | expr '-' expr
    { $$ = (struct expr *)new_expr(2, NULL, $1, $3, "<expr"); }
    | expr '*' expr
    { $$ = (struct expr *)new_expr(3, NULL, $1, $3, "<expr"); }
    | expr '/' expr
    { $$ = (struct expr *)new_expr(4, NULL, $1, $3, "<expr"); }
    ;

value:
      TOK_INT_NUMBER
    { $$ = (struct value *)new_value(0, NULL, NULL, $1.value.intValue, 0, NULL, '\0', "<value"); }
    | TOK_FLOAT_NUMBER
    { $$ = (struct value *)new_value(1, NULL, NULL, 0, $1.value.floatValue, NULL, '\0', "<value"); }
    | TOK_STRING_VALUE
    { $$ = (struct value *)new_value(2, NULL, NULL, 0, 0, $1.value.strValue, '\0', "<value"); }
    | TOK_CHAR_VALUE
    { $$ = (struct value *)new_value(3, NULL, NULL, 0, 0, NULL, $1.value.charValue, "<value"); }
    | TOK_ID
    { $$ = (struct value *)new_value(4, $1, NULL, 0, 0, NULL, '\0', "<value"); }
    | '(' expr ')'
    { $$ = (struct value *)new_value(5, NULL, $2, 0, 0, NULL, '\0', "<value"); }
    /* | '-' expr %prec MINUS_SIGNAL */
    ;

primitive_type:
      TOK_INT
    { $$ = (struct primitive_type *)new_primitive_type(0, "<primitive_type"); }
    | TOK_FLOAT   
    { $$ = (struct primitive_type *)new_primitive_type(1, "<primitive_type"); }
    | TOK_STRING   
    { $$ = (struct primitive_type *)new_primitive_type(2, "<primitive_type"); }
    | TOK_CHAR  
    { $$ = (struct primitive_type *)new_primitive_type(3, "<primitive_type"); }
    ;

if_command:
      TOK_IF '(' boolean_expr ')' '{' statement '}' else_options
    { $$ = (struct if_command *)new_if_command($3, $6, $8, "<if_command"); }
    ;

else_options:
    { $$ = NULL; }
    | TOK_ELSE '{' statement '}' else_options
    { $$ = (struct else_options *)new_else_options(0, $3, $5, NULL, "<else_options"); }
    | TOK_ELSE if_command
    { $$ = (struct else_options *)new_else_options(1, NULL, NULL, $2, "<else_options"); }
    ;

boolean_comparission:
      expr logical_operator expr
    { $$ = (struct boolean_comparission *)new_boolean_comparission($1, $2, "<boolean_comparission"); }
    ;

boolean_expr:
    { $$ = NULL; }
    | boolean_comparission
    { $$ = (struct boolean_expr *)new_boolean_expr(0, $1, NULL, "<boolean_expr"); }
    | TOK_LOG_NOT boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(1, NULL, $2, "<boolean_expr") ; }
    | boolean_expr TOK_LOG_AND boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(2, NULL, $1, "<boolean_expr"); }
    | boolean_expr TOK_LOG_OR boolean_expr
    { $$ = (struct boolean_expr *)new_boolean_expr(3, NULL, $1, "<boolean_expr"); }
    | '(' boolean_expr ')'
    { $$ = (struct boolean_expr *)new_boolean_expr(4, NULL, $2, "<boolean_expr"); }
    ;

logical_operator:
      TOK_GT 
    { $$ = (struct logical_operator *)new_logical_operator(0, "<logical_operator"); }
    | TOK_GET
    { $$ = (struct logical_operator *)new_logical_operator(1, "<logical_operator"); }
    | TOK_LT 
    { $$ = (struct logical_operator *)new_logical_operator(2, "<logical_operator"); }
    | TOK_LET
    { $$ = (struct logical_operator *)new_logical_operator(3, "<logical_operator"); }
    | TOK_EQ 
    { $$ = (struct logical_operator *)new_logical_operator(4, "<logical_operator"); }
    | TOK_DIF
    { $$ = (struct logical_operator *)new_logical_operator(5, "<logical_operator"); }
    ;

while_command:
    TOK_WHILE '(' boolean_expr ')' '{' statement '}'
    { $$ = (struct while_command *)new_while_command($3, $6, "<while_command"); }
    ;

increment:
    TOK_ID TOK_INCR TOK_SEMI
    { $$ = (struct increment *)new_increment($1, "<increment"); }
    ;

%%

void yyerror(const char *s) {
	extern int yylineno;    
	extern char * yytext;   
    printf("Error: %s for symbol '%s' on line %d\n", s, yytext, yylineno);
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    FILE_PATH = argv[1];
    if(!file){
        perror("Error, can't open file\n");
        return 1;
    }

    yyin = file;
    yyparse();
    fclose(file);
    check_errors();
    return 0;
}
