/*
This file is used for mostly for testing the tools that
is going to be used in bison and flex.
*/

#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs/utils.c"

int main() {
    int string_length = 256;
    struct symbol_table *variables = NULL; /* important! initialize to NULL */
    struct symbol_table *s;

    char *var_type_str = processed_input("Enter var type: ", string_length);
    char *var_name = processed_input("Enter var name: ", string_length);
    char *var_value_str = processed_input("Enter var value: ", string_length);

    enum DataType var_type;
    if (strcmp(var_type_str, "int") == 0) {
        var_type = INT;
    } else if (strcmp(var_type_str, "float") == 0) {
        var_type = FLOAT;
    } else if (strcmp(var_type_str, "string") == 0) {
        var_type = STRING;
    } else {
        printf("Unknown type: %s\n", var_type_str);
        return 1;
    }

    add_var(var_name, var_value_str, var_type, &variables);

    s = find_var(var_name, &variables);
    free(var_name);
    free(var_type_str);
    free(var_value_str);

    printf("var name: %s, var type: ", s->key);
    switch (s->type) {
        case INT:
            printf("int, var value: %d\n", s->value.intValue);
            break;
        case FLOAT:
            printf("float, var value: %f\n", s->value.floatValue);
            break;
        case STRING:
            printf("string, var value: %s\n", s->value.strValue);
            break;
    }

    return 0;
}
