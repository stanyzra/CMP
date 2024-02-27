#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

stack *error_stack = NULL;        /* important- initialize to NULL! */
struct hash *symbol_table = NULL; /* important! initialize to NULL */

void push_error(char *error_message) {
    stack *stack_element = malloc(sizeof *stack_element);
    strcpy(stack_element->bname, error_message);
    STACK_PUSH(error_stack, stack_element);
}

void check_stack() {
    stack *stack_element;
    if (!STACK_EMPTY(error_stack)) {
        while (!STACK_EMPTY(error_stack)) {
            printf("%s\n", STACK_TOP(error_stack)->bname);
            STACK_POP(error_stack, stack_element);
            free(stack_element);
        }
    }
}

void trigger_type_error(int expected, int received) {
    extern int yylineno;

    char types[][10] = {"int", "float", "string", "char"};
    char error_message[256];
    sprintf(error_message, "## Error on line %d, expected type '%s', got '%s' ##\n", yylineno, types[expected], types[received]);
    push_error(error_message);
}

void trigger_declaration_first_error(char *token_id) {
    extern int yylineno;

    char error_message[256];
    sprintf(error_message, "## Error on line %d, variable '%s' must be declared first ##\n", yylineno, token_id);
    push_error(error_message);
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
    printf("Adicionando %s - %s\n", key, value);
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