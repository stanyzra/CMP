#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "uthash.h"

enum DataType {
    INT,
    FLOAT,
    STRING,
    CHAR,
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

struct symbol_table {
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
#endif
