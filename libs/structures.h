#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "uthash.h"
#include "utstack.h"
#define BUFLEN 256

enum DataType {
    INT,
    FLOAT,
    STRING,
    CHAR,
    CLASS
};

struct Var_Value {
    enum DataType type;
    union {
        int intValue;
        float floatValue;
        char strValue[128];
        char charValue;
    } value;
};

struct hash {
    char key[128];
    enum DataType type;
    union {
        int intValue;
        float floatValue;
        char strValue[128];
        char charValue;
    } value;
    UT_hash_handle hh; /* makes this structure hashable */
};

typedef struct stack {
    char bname[BUFLEN];
    struct stack *next;
} stack;

#endif
