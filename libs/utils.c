#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

stack *scope_stack = NULL;        /* important- initialize to NULL! */
stack *error_stack = NULL;        /* important- initialize to NULL! */
struct hash *symbol_table = NULL; /* important! initialize to NULL */

void stack_push(char *string, stack **stack_reference) {
    stack *stack_element = malloc(sizeof *stack_element);
    strcpy(stack_element->bname, string);
    STACK_PUSH(*stack_reference, stack_element);
}

void check_errors() {
    stack *stack_element;
    if (!STACK_EMPTY(error_stack)) {
        while (!STACK_EMPTY(error_stack)) {
            printf("%s\n", STACK_TOP(error_stack)->bname);
            STACK_POP(error_stack, stack_element);
            free(stack_element);
        }
    }
}

void print_stack(stack **stack_reference) {
    stack *stack_element;
    if (!STACK_EMPTY(*stack_reference)) {
        while (!STACK_EMPTY(*stack_reference)) {
            printf("vindo da stack: %s\n", STACK_TOP(*stack_reference)->bname);
            STACK_POP(*stack_reference, stack_element);
            free(stack_element);
        }
    }
    // printf("\n");
}

void trigger_type_error(int expected, int received, stack **stack_reference) {
    extern int yylineno;

    char types[][10] = {"int", "float", "string", "char"};
    char error_message[256];
    sprintf(error_message, "## Error on line %d, expected type '%s', got '%s' ##\n", yylineno, types[expected], types[received]);
    stack_push(error_message, stack_reference);
}

void trigger_declaration_first_error(char *token_id, stack **stack_reference) {
    extern int yylineno;

    char error_message[256];
    sprintf(error_message, "## Error on line %d, variable '%s' must be declared first ##\n", yylineno, token_id);
    stack_push(error_message, stack_reference);
}

char *to_string(struct Var_Value structed_value) {
    char *var_to_str;
    if (structed_value.type == INT) {
        int length = snprintf(NULL, 0, "%d", structed_value.value.intValue);
        var_to_str = malloc(sizeof(char) * length);
        sprintf(var_to_str, "%d", structed_value.value.intValue);
    } else if (structed_value.type == FLOAT) {
        int length = snprintf(NULL, 0, "%f", structed_value.value.floatValue);
        var_to_str = malloc(sizeof(char) * length);
        sprintf(var_to_str, "%f", structed_value.value.floatValue);
    } else if (structed_value.type == CHAR) {
        var_to_str = malloc(sizeof(char) * 2);
        var_to_str[0] = structed_value.value.charValue;
        var_to_str[1] = '\0';
        return var_to_str;
    }
    // free(var_to_str);
    return var_to_str;
}

char *processed_input(char *message, int string_length) {
    char *user_string = (char *)malloc(string_length * sizeof(char));
    printf("%s\n", message);
    fgets(user_string, string_length, stdin);
    user_string[strcspn(user_string, "\n")] = '\0';

    return user_string;
}

struct hash *find_element(char *key, struct hash **htable) {
    struct hash *s;
    HASH_FIND_STR(*htable, key, s);
    return s;
}

void add_element(char *key, char *value, enum DataType type, struct hash **htable) {
    struct hash *s;

    if ((s = find_element(key, htable)) == NULL) {
        s = (struct hash *)malloc(sizeof *s);
        strlcpy(s->key, key, sizeof(s->key));
        s->type = type;
        HASH_ADD_STR(*htable, key, s);
    }

    switch (type) {
        case INT:
            s->value.intValue = atoi(value);
            break;
        case FLOAT:
            s->value.floatValue = atof(value);
            break;
        case STRING:
            strlcpy(s->value.strValue, value, sizeof(s->value.strValue));
            break;
        case CHAR:
            s->value.charValue = *value;
            break;
    }
}

void remove_element(struct hash *element, struct hash **htable) {
    HASH_DEL(*htable, element); /* user: pointer to deletee */
    free(element);              /* optional; it's up to you! */
}

extern int yyparse();
extern char *FILE_PATH;
extern FILE *yyin;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern void yypush_buffer_state(YY_BUFFER_STATE new_buffer);
extern void yypop_buffer_state(void);
extern YY_BUFFER_STATE yy_create_buffer(FILE *file, int size);
extern void yyrestart(FILE *input_file);
