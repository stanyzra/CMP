#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

char *processed_input(char *message, int string_length) {
    char *user_string = (char *)malloc(string_length * sizeof(char));
    printf("%s", message);
    fgets(user_string, string_length, stdin);
    user_string[strcspn(user_string, "\n")] = '\0';

    return user_string;
}

struct symbol_table *find_var(char *key, struct symbol_table **variables) {
    struct symbol_table *s;

    HASH_FIND_STR(*variables, key, s);
    return s;
}

void add_var(char *key, char *value, enum DataType type, struct symbol_table **variables) {
    struct symbol_table *s;

    if ((s = find_var(key, variables)) == NULL) {
        s = (struct symbol_table *)malloc(sizeof *s);
        strlcpy(s->key, key, sizeof(s->key));
        s->type = type;
        HASH_ADD_STR(*variables, key, s);
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