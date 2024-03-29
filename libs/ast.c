#include "ast.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <structures.h>

#include <utils.c>

struct ast *new_ast(int node_type, struct ast *left, struct ast *right) {
    struct ast *tree = (struct ast *)malloc(sizeof(struct ast));
    tree->node_type = node_type;
    tree->left = left;
    tree->right = right;

    return tree;
}

struct ast *new_statement(int node_type, struct declaration *declaration, struct becomes *becomes, struct print_command *print_command, struct if_command *if_command, struct while_command *while_command, char *scope) {
    struct statement *tree = (struct statement *)malloc(sizeof(struct statement));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->declaration = declaration;
    tree->becomes = becomes;
    tree->print_command = print_command;
    tree->if_command = if_command;
    tree->while_command = while_command;


    return (struct ast *)tree;
}

struct ast *new_becomes(int node_type, struct expr *expr, struct array_elements *array_elements, struct procedure_call *procedure_call, struct input_call *input_call, char *scope) {
    struct becomes *tree = (struct becomes *)malloc(sizeof(struct becomes));
    tree->node_type = node_type;
    tree->expr = expr;
    tree->array_elements = array_elements;
    tree->procedure_call = procedure_call;
    tree->input_call = input_call;
    return (struct ast *)tree;
}

struct ast *new_print_command(int node_type, struct expr *expr, struct boolean_expr *boolean_expr, struct procedure_call *procedure_call, char *scope) {
    struct print_command *tree = (struct print_command *)malloc(sizeof(struct print_command));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->expr = expr;
    tree->boolean_expr = boolean_expr;
    tree->procedure_call = procedure_call;

    return (struct ast *)tree;
}

struct ast *new_if_command(struct boolean_expr *boolean_expr, struct statement *statement, struct else_options *else_options, char *scope) {
    struct if_command *tree = (struct if_command *)malloc(sizeof(struct if_command));
    // stack_push(scope, &scope_stack);

    tree->boolean_expr = boolean_expr;
    tree->statement = statement;
    tree->else_options = else_options;
    struct hash *symbol_table_element;
    stack *stack_element;

    if (!STACK_EMPTY(scope_stack)) {
        while (!STACK_EMPTY(scope_stack)) {
            char *element_to_remove = STACK_TOP(scope_stack)->bname;
            symbol_table_element = find_element(element_to_remove, &symbol_table);
            remove_element(symbol_table_element, &symbol_table);
            STACK_POP(scope_stack, stack_element);
            free(stack_element);
        }
    }

    return (struct ast *)tree;
}

struct ast *new_while_command(struct boolean_expr *boolean_expr, struct statement *statement, char *scope) {
    struct while_command *tree = (struct while_command *)malloc(sizeof(struct while_command));
    // stack_push(scope, &scope_stack);

    tree->boolean_expr = boolean_expr;
    tree->statement = statement;

    return (struct ast *)tree;
}

struct ast *new_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct becomes *becomes, struct class_type *class_type, struct increment *increment, char *scope) {
    struct declaration *tree = (struct declaration *)malloc(sizeof(struct declaration));
    struct hash *symbol_table_element;

    extern int yylineno;

    char types[][10] = {"int", "float", "string", "char"};

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

    switch (node_type) {
        case 0:
            switch (becomes->node_type) {
                case 0:
                    symbol_table_element = find_element(token_id, &symbol_table);

                    if (symbol_table_element != NULL) {
                        char error_message[256];
                        sprintf(error_message, "## Error on line %d, already defined variable '%s' ##\n", yylineno, token_id);
                        printf("%s\n", scope);
                        stack_push(error_message, &error_stack);
                    } else if (primitive_type->node_type == becomes->expr->value->node_type) {
                        // Atribuição normal
                        stack_push(token_id, &scope_stack);

                        switch (becomes->expr->value->node_type) {
                            struct Var_Value structed_value;
                            case 0:
                                structed_value.type = INT;
                                structed_value.value.intValue = becomes->expr->value->int_value;
                                add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                break;
                            case 1:
                                structed_value.type = FLOAT;
                                structed_value.value.floatValue = becomes->expr->value->float_value;
                                add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                break;
                            case 2:
                                add_element(token_id, becomes->expr->value->string_value, STRING, &symbol_table);
                                break;
                            case 3:
                                structed_value.type = CHAR;
                                structed_value.value.charValue = becomes->expr->value->char_value;
                                add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                break;

                            default:
                                printf("Erro\n");
                                exit(1);
                                break;
                        }
                    } else if (becomes->expr->value->token_id != NULL && becomes->expr->value->node_type == 4) {
                        // Atribuição com variável
                        symbol_table_element = find_element(becomes->expr->value->token_id, &symbol_table);

                        if (symbol_table_element != NULL) {
                            if (primitive_type->node_type == symbol_table_element->type) {
                                switch (symbol_table_element->type) {
                                    struct Var_Value structed_value;
                                    case 0:
                                        structed_value.type = INT;
                                        structed_value.value.intValue = becomes->expr->value->int_value;
                                        add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                        break;
                                    case 1:
                                        structed_value.type = FLOAT;
                                        structed_value.value.floatValue = becomes->expr->value->float_value;
                                        add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                        break;
                                    case 2:
                                        add_element(token_id, becomes->expr->value->string_value, STRING, &symbol_table);
                                        break;
                                    case 3:
                                        structed_value.type = CHAR;
                                        structed_value.value.charValue = becomes->expr->value->char_value;
                                        add_element(token_id, to_string(structed_value), structed_value.type, &symbol_table);
                                        break;

                                    default:
                                        printf("Erro\n");
                                        exit(1);
                                        break;
                                }
                            }
                        } else {
                            char error_message[256];
                            sprintf(error_message, "## Error on line %d, undefined variable '%s' ##\n", yylineno, becomes->expr->value->token_id);
                            stack_push(error_message, &error_stack);
                        }
                    } else {
                        trigger_type_error(primitive_type->node_type, becomes->expr->value->node_type, &error_stack);
                    }
                    break;
                case 1:  // declação de array
                    if ((becomes->array_elements != NULL)) {
                        if (primitive_type->node_type == becomes->array_elements->expr->value->node_type) {
                            // inserir na tabela de simbolos
                        } else {
                            int declaration_type = primitive_type->node_type;
                            int attr_type = becomes->array_elements->expr->value->node_type;

                            char error_message[256];
                            sprintf(error_message, "## Error on line %d, expected array of type '%s', got value(s) of type '%s' ##\n", yylineno, types[declaration_type], types[attr_type]);
                            stack_push(error_message, &error_stack);
                        }
                    }
                    break;
                case 2:  // declação de variavel por resultado de função
                    if ((becomes->procedure_call != NULL)) {
                        symbol_table_element = find_element(becomes->procedure_call->token_id, &symbol_table);

                        if (symbol_table_element != NULL) {
                            if (primitive_type->node_type == symbol_table_element->type) {
                                // inserir na tabela de simbolos
                            } else {
                                trigger_type_error(primitive_type->node_type, symbol_table_element->type, &error_stack);
                            }
                        } else {
                            char error_message[256];
                            sprintf(error_message, "## Error on line %d, undefined procedure '%s' ##\n", yylineno, becomes->procedure_call->token_id);
                            stack_push(error_message, &error_stack);
                        }
                    }
                    break;
                case 3:  // declaração de variavel por função de input
                    if (primitive_type->node_type == STRING) {
                        // inserir na tabela de simbolos
                    } else {
                        trigger_type_error(STRING, primitive_type->node_type, &error_stack);
                    }
                    break;
                default:
                    break;
            }
            break;
        case 1:  // declaração de objeto
            symbol_table_element = find_element(class_type->token_id, &symbol_table);
            if (becomes->node_type == 2 && strcmp(class_type->token_id, becomes->procedure_call->token_id) == 0) {
            } else {
                char error_message[256];
                sprintf(error_message, "## Error on line %d, expected class '%s', got '%s' ##\n", yylineno, class_type->token_id, becomes->procedure_call->token_id);
                stack_push(error_message, &error_stack);
            }
            break;
        case 2:  // incremento
            symbol_table_element = find_element(increment->token_id, &symbol_table);
            if (symbol_table_element != NULL) {
                if (symbol_table_element->type == INT) {
                    // inserir na tabela de simbolos

                } else {
                    trigger_type_error(INT, symbol_table_element->type, &error_stack);
                }
            } else {
                trigger_declaration_first_error(increment->token_id, &error_stack);
            }
            break;
        case 3:  // reatribuição de variável
            symbol_table_element = find_element(token_id, &symbol_table);

            if (symbol_table_element != NULL) {
                if (symbol_table_element->type == becomes->expr->value->node_type) {
                    // inserir na tabela de simbolos

                } else {
                    trigger_type_error(symbol_table_element->type, becomes->expr->value->node_type, &error_stack);
                }
            } else {
                trigger_declaration_first_error(token_id, &error_stack);
            }
            break;

        default:

            printf("Erro\n");
            exit(1);
            break;
    }

    return (struct ast *)tree;
}

void parse_file(char *path) {
    path[strlen(path) - 1] = 0;

    FILE *file = fopen(path + 1, "r");
    if (!file) {
        perror("Error opening the file\n");
        exit(1);
    }
    yyin = file;
    yyrestart(yyin);

    FILE *original_file = fopen(FILE_PATH, "r");
    if (!original_file) {
        perror("Error opening the original file\n");
        exit(1);
    }
    yyin = original_file;
    yyrestart(yyin);
}

struct ast *new_import_command(struct expr *expr, char *scope) {
    struct import_command *tree = (struct import_command *)malloc(sizeof(struct import_command));
    // stack_push(scope, &scope_stack);

    tree->expr = expr;

    if (expr != NULL) {
        parse_file(expr->value->string_value);
    }

    return (struct ast *)tree;
}

struct ast *new_imports(struct imports *imports, struct import_command *import_command, char *scope) {
    struct imports *tree = (struct imports *)malloc(sizeof(struct imports));
    // stack_push(scope, &scope_stack);

    tree->imports = imports;
    tree->import_command = import_command;

    return (struct ast *)tree;
}

struct ast *new_arguments_declaration(int node_type, char *token_id, struct primitive_type *primitive_type, struct class_type *class_type, struct arguments_declaration *arguments_declaration, char *scope) {
    struct arguments_declaration *tree = (struct arguments_declaration *)malloc(sizeof(struct arguments_declaration));
    // stack_push(scope, &scope_stack);

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

struct ast *new_procedure(char *token_id, struct arguments_declaration *arguments_declaration, struct primitive_type *primitive_type, struct statement *statement, struct procedure_return *procedure_return, char *scope) {
    struct procedure *tree = (struct procedure *)malloc(sizeof(struct procedure));

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
        add_element(token_id, "proc", primitive_type->node_type, &symbol_table);
    } else {
        strcpy(tree->token_id, "");
    }
    stack_push(token_id, &scope_stack);
    tree->arguments_declaration = arguments_declaration;
    tree->primitive_type = primitive_type;
    tree->statement = statement;
    tree->procedure_return = procedure_return;

    return (struct ast *)tree;
}

struct ast *new_procedure_return(int node_type, struct expr *expr, struct boolean_expr *boolean_expr, char *scope) {
    struct procedure_return *tree = (struct procedure_return *)malloc(sizeof(struct procedure_return));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->expr = expr;
    tree->boolean_expr = boolean_expr;

    return (struct ast *)tree;
}

struct ast *new_procedures(struct procedure *procedure, struct procedures *procedures, char *scope) {
    struct procedures *tree = (struct procedures *)malloc(sizeof(struct procedures));
    // stack_push(scope, &scope_stack);

    tree->procedure = procedure;
    tree->procedures = procedures;

    return (struct ast *)tree;
}

struct ast *new_class_declaration(char *token_id, struct declaration *declaration, struct statement *statement, char *scope) {
    struct class_declaration *tree = (struct class_declaration *)malloc(sizeof(struct class_declaration));
    struct hash *symbol_table_element;
    stack *stack_element;

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }

    tree->declaration = declaration;
    tree->statement = statement;

    if (!STACK_EMPTY(scope_stack)) {
        while (!STACK_EMPTY(scope_stack)) {
            char *element_to_remove = STACK_TOP(scope_stack)->bname;
            symbol_table_element = find_element(element_to_remove, &symbol_table);
            remove_element(symbol_table_element, &symbol_table);
            STACK_POP(scope_stack, stack_element);
            free(stack_element);
        }
    }

    return (struct ast *)tree;
}

struct ast *new_classes(struct class_declaration *class_declaration, struct classes *classes, char *scope) {
    struct classes *tree = (struct classes *)malloc(sizeof(struct classes));
    // stack_push(scope, &scope_stack);

    tree->class_declaration = class_declaration;
    tree->classes = classes;

    return (struct ast *)tree;
}

struct ast *new_input_call(struct expr *expr, char *scope) {
    struct input_call *tree = (struct input_call *)malloc(sizeof(struct input_call));
    // stack_push(scope, &scope_stack);

    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_class_type(char *token_id, char *scope) {
    struct class_type *tree = (struct class_type *)malloc(sizeof(struct class_type));
    // stack_push(token_id, &scope_stack);

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
        add_element(token_id, token_id, CLASS, &symbol_table);
    } else {
        strcpy(tree->token_id, "");
    }

    return (struct ast *)tree;
}

struct ast *new_procedure_call(char *token_id, struct arguments *arguments, char *scope) {
    struct procedure_call *tree = (struct procedure_call *)malloc(sizeof(struct procedure_call));
    stack_push(token_id, &scope_stack);

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
    } else {
        strcpy(tree->token_id, "");
    }
    tree->arguments = arguments;

    return (struct ast *)tree;
}

struct ast *new_arguments(int node_type, struct expr *expr, struct arguments *arguments, char *scope) {
    struct arguments *tree = (struct arguments *)malloc(sizeof(struct arguments));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->expr = expr;
    tree->arguments = arguments;

    return (struct ast *)tree;
}

struct ast *new_array_elements(int node_type, struct expr *expr, struct array_elements *array_elements, char *scope) {
    struct array_elements *tree = (struct array_elements *)malloc(sizeof(struct array_elements));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->expr = expr;
    tree->array_elements = array_elements;

    return (struct ast *)tree;
}

struct ast *new_expr(int node_type, struct value *value, struct expr *expr_1, struct expr *expr_2, char *scope) {
    struct expr *tree = (struct expr *)malloc(sizeof(struct expr));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->value = value;
    tree->expr_1 = expr_1;
    tree->expr_2 = expr_2;

    return (struct ast *)tree;
}

struct ast *new_value(int node_type, char *token_id, struct expr *expr, int int_value, float float_value, char *str_value, char char_value, char *scope) {
    struct value *tree = (struct value *)malloc(sizeof(struct value));

    tree->node_type = node_type;
    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
        // stack_push(token_id, &scope_stack);
    } else
        strcpy(tree->token_id, "");

    if (str_value != NULL)
        strcpy(tree->string_value, str_value);
    else
        strcpy(tree->string_value, "");

    tree->char_value = char_value;
    tree->float_value = float_value;
    tree->int_value = int_value;
    tree->expr = expr;

    return (struct ast *)tree;
}

struct ast *new_primitive_type(int node_type, char *scope) {
    struct primitive_type *tree = (struct primitive_type *)malloc(sizeof(struct primitive_type));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;

    return (struct ast *)tree;
}

struct ast *new_else_options(int node_type, struct statement *statement, struct else_options *else_options, struct if_command *if_command, char *scope) {
    struct else_options *tree = (struct else_options *)malloc(sizeof(struct else_options));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->statement = statement;
    tree->else_options = else_options;
    tree->if_command = if_command;

    struct hash *symbol_table_element;
    stack *stack_element;
    if (!STACK_EMPTY(scope_stack)) {
        while (!STACK_EMPTY(scope_stack)) {
            char *element_to_remove = STACK_TOP(scope_stack)->bname;
            symbol_table_element = find_element(element_to_remove, &symbol_table);
            remove_element(symbol_table_element, &symbol_table);
            STACK_POP(scope_stack, stack_element);
            free(stack_element);
        }

        // STACK_POP(error_stack, stack_element);
    }

    return (struct ast *)tree;
}

struct ast *new_boolean_comparission(struct expr *expr, struct logical_operator *logical_operator, char *scope) {
    struct boolean_comparission *tree = (struct boolean_comparission *)malloc(sizeof(struct boolean_comparission));
    // stack_push(scope, &scope_stack);

    tree->expr = expr;
    tree->logical_operator = logical_operator;

    return (struct ast *)tree;
}

struct ast *new_boolean_expr(int node_type, struct boolean_comparission *boolean_comparission, struct boolean_expr *boolean_expr, char *scope) {
    struct boolean_expr *tree = (struct boolean_expr *)malloc(sizeof(struct boolean_expr));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;
    tree->boolean_comparission = boolean_comparission;
    tree->boolean_expr = boolean_expr;

    return (struct ast *)tree;
}

struct ast *new_logical_operator(int node_type, char *scope) {
    struct logical_operator *tree = (struct logical_operator *)malloc(sizeof(struct logical_operator));
    // stack_push(scope, &scope_stack);

    tree->node_type = node_type;

    return (struct ast *)tree;
}

struct ast *new_increment(char *token_id, char *scope) {
    struct increment *tree = (struct increment *)malloc(sizeof(struct increment));
    struct hash *element;

    if (token_id != NULL) {
        strcpy(tree->token_id, token_id);
        element = find_element(token_id, &symbol_table);
        // stack_push(token_id, &scope_stack);
    } else {
        strcpy(tree->token_id, "");
    }

    return (struct ast *)tree;
}

struct ast *new_main(struct statement *statement, char *scope) {
    struct main *tree = (struct main *)malloc(sizeof(struct main));
    // stack_push("<main", &scope_stack);

    tree->statement = statement;

    return (struct ast *)tree;
}

struct ast *new_program(int node_type, struct imports *imports, struct classes *classes, struct procedures *procedures, struct main *main, char *scope) {
    struct program *tree = (struct program *)malloc(sizeof(struct program));
    // stack_push(scope, &scope_stack);

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

void print_tree(struct ast *tree) {
    if (tree == NULL)
        return;

    printf("%d\n", tree->node_type);

    print_tree(tree->left);

    print_tree(tree->right);
}

void traverse_imports(struct imports *imports, char *scope) {
    if (imports != NULL) {
        new_import_command(imports->import_command->expr, scope);
        traverse_imports(imports->imports, scope);
    }
}
void traverse_classes(struct classes *classes, char *scope) {
    if (classes != NULL) {
        new_class_declaration(classes->class_declaration->token_id, classes->class_declaration->declaration, classes->class_declaration->statement, scope);
        traverse_classes(classes->classes, scope);
    }
}
void traverse_prodecures(struct procedures *procedures, char *scope) {
    if (procedures != NULL) {
        new_procedure(procedures->procedure->token_id, procedures->procedure->arguments_declaration, procedures->procedure->primitive_type, procedures->procedure->statement, procedures->procedure->procedure_return, scope);
        traverse_prodecures(procedures->procedures, scope);
    }
}
void traverse_main(struct main *main, char *scope) {

    if (main != NULL) {
        new_statement(main->statement->node_type, main->statement->declaration, main->statement->becomes, main->statement->print_command, main->statement->if_command, main->statement->while_command, scope);
    }
}