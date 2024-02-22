%{
// #include "symbol_table.h" // Include symbol table header
%}

%%
INTEGER [0-9]+    { 
    yylval.ival = atoi(yytext); 
    return INTEGER; 
}

FLOAT   [+-]?([0-9]+(\.[0-9]*)?|\.[0-9]+)([eE][+-]?[0-9]+)? {
    yylval.fval = atof(yytext); return FLOAT;
}

STRING  "([^\"\\]|\\.)*"   {
    yylval.sval = strdup(yytext + 1);
    yylval.slen = strlen(yytext) - 2;
    return STRING;
}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]* {
    if (is_keyword(yytext)) {
        return KEYWORD;
    }
    add_symbol(yytext, IDENTIFIER);
    return IDENTIFIER;
}

INVALID .+   {
    fprintf(stderr, "Error: Invalid character '%s' at line %d\n", yytext, yylineno);
    return -1; // Handle error appropriately
}

%%