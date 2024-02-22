#include <stdio.h>
#include "lex.yy.c" // Include generated header

int main()
{
    yylex(); // Start lexical analysis
    while (yylex() != -1)
    { // Process tokens
        switch (yylval.type)
        {
        case INTEGER:
            printf("Integer: %d\n", yylval.ival);
            break;
        case FLOAT:
            printf("Float: %f\n", yylval.fval);
            break;
        case STRING:
            printf("String: %s\n", yylval.sval);
            free(yylval.sval); // Free dynamically allocated string
            break;
        case IDENTIFIER:
            printf("Identifier: %s\n", yylval.text);
            break;
        // Add cases for other token types
        default:
            printf("Unknown token\n");
            break;
        }
    }
    return 0;
}
