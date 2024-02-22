#include <stdio.h>
#include "uthash.h"
#include <string.h>
#include <stdlib.h>

struct symbol_table
{
    char key[128]; /* key */
    char value[128];
    UT_hash_handle hh; /* makes this structure hashable */
};

struct symbol_table *tokens = NULL; /* important! initialize to NULL */

struct symbol_table *find_token(char *key)
{
    struct symbol_table *s;

    HASH_FIND_STR(tokens, key, s);
    return s;
}

void add_token(char *key, char *value)
{
    struct symbol_table *s;
    s = (struct symbol_table *)malloc(sizeof *s);
    strcpy(s->key, key);
    strcpy(s->value, value);
    HASH_ADD_STR(tokens, key, s);
}

int main()
{
    int string_length = 256;
    char user_string[string_length];

    struct symbol_table *s;
    add_token("token_teste", "34");

    printf("Enter your token: ");
    fgets(user_string, string_length, stdin);
    user_string[strcspn(user_string, "\n")] = '\0';

    s = find_token(user_string);
    printf("token: %s\n", s ? s->value : "unknown");
    return 0;
}
