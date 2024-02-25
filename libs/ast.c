#include "ast.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast *new_ast(int node_type, struct ast *left, struct ast *right) {
    struct ast *tree = (struct ast *)malloc(sizeof(struct ast));

    tree->node_type = node_type;
    tree->left = left;
    tree->right = right;

    return tree;
}

struct ast *new_statement(int node_type, struct declaration *declaration, struct becomes *becomes, struct print_command *print_command, struct if_command *if_command, struct while_command *while_command) {
    struct statement *tree = (struct statement *)malloc(sizeof(struct statement));
}

struct ast *new_becomes(int node_type, struct expr *expr, struct array_elements *array_elements, struct procedure_call *procedure_call, struct input_call *input_call) {
    struct becomes *tree = (struct becomes *)malloc(sizeof(struct becomes));

    tree->node_type = node_type;
    tree->expr = expr;
    tree->array_elements = array_elements;
    tree->procedure_call = procedure_call;
    tree->input_call = input_call;
    printf("print new becomes");
    return (struct ast *)tree;
}

struct ast *new_print_command(int node_type, struct expr *expr, struct boolean_expr *boolean_expr, struct procedure_call *procedure_call) {
    struct print_command *tree = (struct print_command *)malloc(sizeof(struct print_command));

    tree->node_type = node_type;
    tree->expr = expr;
    tree->boolean_expr = boolean_expr;
    tree->procedure_call = procedure_call;

    return (struct ast *)tree;
}

struct ast *new_if_command(struct boolean_expr *boolean_expr, struct statement *statement, struct else_options *else_options) {
    struct if_command *tree = (struct if_command *)malloc(sizeof(struct if_command));

    tree->boolean_expr = boolean_expr;
    tree->statement = statement;
    tree->else_options = else_options;

    return (struct ast *)tree;
}

struct ast *new_while_command(struct boolean_expr *boolean_expr, struct statement *statement) {
    printf("teste new while comdm");
    struct while_command *tree = (struct while_command *)malloc(sizeof(struct while_command));

    tree->boolean_expr = boolean_expr;
    tree->statement = statement;

    return (struct ast *)tree;
}

struct ast *new_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct becomes *becomes, struct class_type *class_type, struct increment *increment) {
    struct declaration *tree = (struct declaration *)malloc(sizeof(struct declaration));

    tree->node_type = node_type;
    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->primitive_type = primitive_type;
    tree->becomes = becomes;
    tree->class_type = class_type;
    tree->increment = increment;
    return (struct ast *)tree;
}

struct ast *new_import_command(struct expr *expr) {
    struct import_command *tree = (struct import_command *)malloc(sizeof(struct import_command));

    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_imports(struct imports *imports, struct import_command *import_command) {
    struct imports *tree = (struct imports *)malloc(sizeof(struct imports));

    tree->imports = imports;
    tree->import_command = import_command;

    return (struct ast *)tree;
}

struct ast *new_arguments_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct class_type *class_type, struct arguments_declaration *arguments_declaration) {
    struct arguments_declaration *tree = (struct arguments_declaration *)malloc(sizeof(struct arguments_declaration));

    tree->node_type = node_type;
    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->primitive_type = primitive_type;
    tree->class_type = class_type;
    tree->arguments_declaration = arguments_declaration;

    return (struct ast *)tree;
}

struct ast *new_procedure(char *token_id, struct arguments_declaration *arguments_declaration, struct primitive_type *primitive_type, struct statement *statement, struct procedure_return *procedure_return) {
    struct procedure *tree = (struct procedure *)malloc(sizeof(struct procedure));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->arguments_declaration = arguments_declaration;
    tree->primitive_type = primitive_type;
    tree->statement = statement;
    tree->procedure_return = procedure_return;

    return (struct ast *)tree;
}

struct ast *new_procedure_return(int node_type, struct expr *expr, struct boolean_expr *boolean_expr) {
    struct procedure_return *tree = (struct procedure_return *)malloc(sizeof(struct procedure_return));

    tree->node_type = node_type;
    tree->expr = expr;
    tree->boolean_expr = boolean_expr;

    return (struct ast *)tree;
}

struct ast *new_procedures(struct procedure *procedure, struct procedures *procedures) {
    struct procedures *tree = (struct procedures *)malloc(sizeof(struct procedures));

    tree->procedure = procedure;
    tree->procedures = procedures;

    return (struct ast *)tree;
}

struct ast *new_class_declaration(char *token_id, struct declaration *declaration, struct statement *statement) {
    struct class_declaration *tree = (struct class_declaration *)malloc(sizeof(struct class_declaration));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->declaration = declaration;
    tree->statement = statement;

    return (struct ast *)tree;
}

struct ast *new_classes(struct class_declaration *class_declaration, struct classes *classes) {
    struct classes *tree = (struct classes *)malloc(sizeof(struct classes));

    tree->class_declaration = class_declaration;
    tree->classes = classes;

    return (struct ast *)tree;
}

struct ast *new_input_call(struct expr *expr) {
    struct input_call *tree = (struct input_call *)malloc(sizeof(struct input_call));

    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_class_type(char *token_id) {
    struct class_type *tree = (struct class_type *)malloc(sizeof(struct class_type));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }

    return (struct ast *)tree;
}

struct ast *new_procedure_call(char *token_id, struct arguments *arguments) {
    struct procedure_call *tree = (struct procedure_call *)malloc(sizeof(struct procedure_call));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->arguments = arguments;

    return (struct ast *)tree;
}

struct ast *new_arguments(int node_type, struct expr *expr, struct arguments *arguments) {
    struct arguments *tree = (struct arguments *)malloc(sizeof(struct arguments));

    tree->node_type = node_type;
    tree->expr = expr;
    tree->arguments = arguments;

    return (struct ast *)tree;
}

struct ast *new_array_elements(int node_type, struct expr *expr, struct array_elements *array_elements) {
    struct array_elements *tree = (struct array_elements *)malloc(sizeof(struct array_elements));

    tree->node_type = node_type;
    tree->expr = expr;
    tree->array_elements = array_elements;

    return (struct ast *)tree;
}

struct ast *new_expr(int node_type, struct value *value, struct expr *expr) {
    printf("teste new expr\n");

    struct expr *tree = (struct expr *)malloc(sizeof(struct expr));

    tree->node_type = node_type;
    tree->value = value;
    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_value(int node_type, char *token_id, struct expr *expr) {
    printf("teste new_value\n");

    struct value *tree = (struct value *)malloc(sizeof(struct value));

    tree->node_type = node_type;
    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_primitive_type(int node_type) {
    struct primitive_type *tree = (struct primitive_type *)malloc(sizeof(struct primitive_type));

    tree->node_type = node_type;

    return (struct ast *)tree;
}

struct ast *new_else_options(int node_type, struct statement *statement, struct else_options *else_options, struct if_command *if_command) {
    struct else_options *tree = (struct else_options *)malloc(sizeof(struct else_options));

    tree->node_type = node_type;
    tree->statement = statement;
    tree->else_options = else_options;
    tree->if_command = if_command;

    return (struct ast *)tree;
}

struct ast *new_boolean_comparission(struct expr *expr, struct logical_operator *logical_operator) {
    struct boolean_comparission *tree = (struct boolean_comparission *)malloc(sizeof(struct boolean_comparission));

    tree->expr = expr;
    tree->logical_operator = logical_operator;

    return (struct ast *)tree;
}

struct ast *new_boolean_expr(int node_type, struct boolean_comparission *boolean_comparission, struct boolean_expr *boolean_expr) {
    struct boolean_expr *tree = (struct boolean_expr *)malloc(sizeof(struct boolean_expr));

    tree->node_type = node_type;
    tree->boolean_comparission = boolean_comparission;
    tree->boolean_expr = boolean_expr;

    return (struct ast *)tree;
}

struct ast *new_logical_operator(int node_type) {
    struct logical_operator *tree = (struct logical_operator *)malloc(sizeof(struct logical_operator));

    tree->node_type = node_type;

    return (struct ast *)tree;
}

struct ast *new_increment(char *token_id) {
    struct increment *tree = (struct increment *)malloc(sizeof(struct increment));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }

    return (struct ast *)tree;
}

struct ast *new_main(struct statement *statement) {
    printf("teste new main");
    struct main *tree = (struct main *)malloc(sizeof(struct main));

    tree->statement = statement;

    return (struct ast *)tree;
}

struct ast *new_program(int node_type, struct imports *imports, struct classes *classes, struct procedures *procedures, struct main *main) {
    struct program *tree = (struct program *)malloc(sizeof(struct program));

    tree->node_type = node_type;
    tree->imports = imports;
    tree->classes = classes;
    tree->procedures = procedures;
    tree->main = main;

    return (struct ast *)tree;
}

/*CRIAR UMA FUNÇÃO DE VISITAR ARVORE PARA CADA
REGRA DE PROGRAM E DESENVOLVER A SEMANTICA DENTRO
DAS RESPECTIVAS FUNÇÕES
*/

void create_tree(struct ast *tree) {
    printf("PRINT CREATE TREE\n");
    if (tree == NULL)
        return;

    // First print data of node
    printf("%d\n", tree->node_type);

    // Then recur on left subtree
    create_tree(tree->left);

    // Now recur on right subtree
    create_tree(tree->right);
}