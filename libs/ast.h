
struct program {
    int node_type;

    struct imports *imports;
    struct classes *classes;
    struct procedures *procedures;
    struct main *main;
};

struct statement {
    int node_type;

    struct declaration *declaration;
    struct becomes *becomes;
    struct print_command *print_command;
    struct if_command *if_command;
    struct while_command *while_command;
};

struct becomes {
    int node_type;

    struct expr *expr;
    struct array_elements *array_elements;
    struct procedure_call *procedure_call;
    struct input_call *input_call;
};

struct if_command {
    struct boolean_expr *boolean_expr;
    struct statement *statement;
    struct else_options *else_options;
};

struct print_command {
    int node_type;

    struct expr *expr;
    struct boolean_expr *boolean_expr;
    struct procedure_call *procedure_call;
};

struct while_command {
    struct boolean_expr *boolean_expr;
    struct statement *statement;
};

struct declaration {
    int node_type;
    char token_id[64];

    struct primitive_type *primitive_type;
    struct becomes *becomes;
    struct class_type *class_type;
    struct increment *increment;
};

struct import_command {
    struct expr *expr;
};

struct imports {
    struct imports *imports;
    struct import_command *import_command;
};

struct arguments_declaration {
    int node_type;
    char token_id[64];

    struct primitive_type *primitive_type;
    struct class_type *class_type;
    struct arguments_declaration *arguments_declaration;
};

struct procedure {
    char token_id[64];

    struct arguments_declaration *arguments_declaration;
    struct primitive_type *primitive_type;
    struct statement *statement;
    struct procedure_return *procedure_return;
};

struct procedure_return {
    int node_type;

    struct expr *expr;
    struct boolean_expr *boolean_expr;
};

struct procedures {
    struct procedure *procedure;
    struct procedures *procedures;
};

struct class_declaration {
    char token_id[64];

    struct declaration *declaration;
    struct statement *statement;
};

struct classes {
    struct class_declaration *class_declaration;
    struct classes *classes;
};

struct input_call {
    struct expr *expr;
};

struct class_type {
    char token_id[64];
};

struct procedure_call {
    char token_id[64];

    struct arguments *arguments;
};

struct arguments {
    int node_type;

    struct expr *expr;
    struct arguments *arguments;
};

struct array_elements {
    int node_type;

    struct expr *expr;
    struct array_elements *array_elements;
};

struct expr {
    int node_type;

    struct value *value;
    struct expr *expr_1;
    struct expr *expr_2;
};

struct value {
    int node_type;
    char token_id[64];

    struct expr *expr;
    int int_value;
    float float_value;
    char string_value[256];
    char char_value;
};

struct primitive_type {
    int node_type;
};

struct else_options {
    int node_type;

    struct statement *statement;
    struct else_options *else_options;
    struct if_command *if_command;
};

struct boolean_comparission {
    struct expr *expr;
    struct logical_operator *logical_operator;
};

struct boolean_expr {
    int node_type;

    struct boolean_comparission *boolean_comparission;
    struct boolean_expr *boolean_expr;
};

struct logical_operator {
    int node_type;
};

struct increment {
    char token_id[64];
};

struct main {
    struct statement *statement;
};

struct ast {
    int node_type;

    struct ast *left;
    struct ast *right;
};

struct ast *new_ast(int node_type, struct ast *left, struct ast *right);
struct ast *new_statement(int node_type, struct declaration *declaration, struct becomes *becomes, struct print_command *print_command, struct if_command *if_command, struct while_command *while_command, char *scope);
struct ast *new_becomes(int node_type, struct expr *expr, struct array_elements *array_elements, struct procedure_call *procedure_call, struct input_call *input_call, char *scope);
struct ast *new_print_command(int node_type, struct expr *expr, struct boolean_expr *boolean_expr, struct procedure_call *procedure_call, char *scope);
struct ast *new_if_command(struct boolean_expr *boolean_expr, struct statement *statement, struct else_options *else_options, char *scope);
struct ast *new_while_command(struct boolean_expr *boolean_expr, struct statement *statement, char *scope);
struct ast *new_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct becomes *becomes, struct class_type *class_type, struct increment *increment, char *scope);
struct ast *new_import_command(struct expr *expr, char *scope);
struct ast *new_imports(struct imports *imports, struct import_command *import_command, char *scope);
struct ast *new_arguments_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct class_type *class_type, struct arguments_declaration *arguments_declaration, char *scope);
struct ast *new_procedure(char *token_id, struct arguments_declaration *arguments_declaration, struct primitive_type *primitive_type, struct statement *statement, struct procedure_return *procedure_return, char *scope);
struct ast *new_procedure_return(int node_type, struct expr *expr, struct boolean_expr *boolean_expr, char *scope);
struct ast *new_procedures(struct procedure *procedure, struct procedures *procedures, char *scope);
struct ast *new_class_declaration(char *token_id, struct declaration *declaration, struct statement *statement, char *scope);
struct ast *new_classes(struct class_declaration *class_declaration, struct classes *classes, char *scope);
struct ast *new_input_call(struct expr *expr, char *scope);
struct ast *new_class_type(char *token_id, char *scope);
struct ast *new_procedure_call(char *token_id, struct arguments *arguments, char *scope);
struct ast *new_arguments(int node_type, struct expr *expr, struct arguments *arguments, char *scope);
struct ast *new_array_elements(int node_type, struct expr *expr, struct array_elements *array_elements, char *scope);
struct ast *new_expr(int node_type, struct value *value, struct expr *expr_1, struct expr *expr_2, char *scope);
struct ast *new_value(int node_type, char *token_id, struct expr *expr, int int_value, float float_value, char *str_value, char char_value, char *scope);
struct ast *new_primitive_type(int node_type, char *scope);
struct ast *new_else_options(int node_type, struct statement *statement, struct else_options *else_options, struct if_command *if_command, char *scope);
struct ast *new_boolean_comparission(struct expr *expr, struct logical_operator *logical_operator, char *scope);
struct ast *new_boolean_expr(int node_type, struct boolean_comparission *boolean_comparission, struct boolean_expr *boolean_expr, char *scope);
struct ast *new_logical_operator(int node_type, char *scope);
struct ast *new_increment(char *token_id, char *scope);
struct ast *new_main(struct statement *statement, char *scope);
struct ast *new_program(int node_type, struct imports *imports, struct classes *classes, struct procedures *procedures, struct main *main, char *scope);