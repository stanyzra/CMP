%{
#include <stdio.h>
#include <stdlib.h>
#include "libs/utils.c"

int yylex(void);
int yyparse(void);
void yyerror(const char *s);

struct symbol_table *variables = NULL; /* important! initialize to NULL */

%}

%union {
    struct Var_Value var_val;
    char id[64];
}

%token <id> TOK_ID
%token <var_val> TOK_INT_NUMBER
%token <var_val> TOK_FLOAT_NUMBER
%token <var_val> TOK_STRING_VALUE
%token <var_val> TOK_CHAR_VALUE

%type <var_val> expr

%token TOK_INT TOK_FLOAT TOK_CHAR TOK_STRING TOK_SEMI TOK_PRINT

%left '+' '-'
%left '*' '/'
%nonassoc MINUS_SIGNAL

%%

program:
    | program statement
    ;

statement:
      expr TOK_SEMI
    | type TOK_ID '=' expr TOK_SEMI         
    {
        if ($4.type == INT) {
            int length = snprintf(NULL, 0, "%d", $4.value.intValue);
            char int_to_str[length + 1];
            sprintf(int_to_str, "%d", $4.value.intValue);
            add_var($2, int_to_str, $4.type, &variables);
        } else if ($4.type == FLOAT) {
            int length = snprintf(NULL, 0, "%f", $4.value.floatValue);
            char flt_to_str[length + 1];
            sprintf(flt_to_str, "%f", $4.value.floatValue);
            add_var($2, flt_to_str, $4.type, &variables);
        } else if ($4.type == STRING) {
            printf("Str value: %s", $4.value.strValue);
            add_var($2, $4.value.strValue, $4.type, &variables);
        } else if ($4.type == CHAR) {
            char chr_to_str[2];
            chr_to_str[0] = $4.value.charValue;
            chr_to_str[1] = '\0';
            add_var($2, chr_to_str, $4.type, &variables);
        }
    }
    | TOK_PRINT '(' expr ')' TOK_SEMI
    {
        if ($3.type == INT) {
            printf("%d\n", $3.value.intValue);
        } else if ($3.type == FLOAT) {
            printf("%f\n", $3.value.floatValue);
        } else if ($3.type == STRING) {
            printf("%s\n", $3.value.strValue);
        } else if ($3.type == CHAR) {
            printf("%c\n", $3.value.charValue);
        }
    }
    ;

expr:
      TOK_INT_NUMBER
    | TOK_FLOAT_NUMBER
    | TOK_STRING_VALUE
    | TOK_CHAR_VALUE
    | TOK_ID 
    {
        struct Var_Value id;
        struct symbol_table *s;
        s = find_var($1, &variables);
        id.type = s->type;
        if (s->type == INT) {
            id.value.intValue = s->value.intValue;
        } else if (s->type == FLOAT) {
            id.value.floatValue = s->value.floatValue;
        } else if (s->type == STRING) {
            strcpy(id.value.strValue, s->value.strValue);
        } else if (s->type == CHAR) {
            id.value.charValue = s->value.charValue;
        }

        $$ = id;
    }
    | expr '+' expr
    {
        if ($1.type == INT && $3.type == INT) {
            $$.type = INT; 
            $$.value.intValue = $1.value.intValue + $3.value.intValue; 
        } else if ($1.type == FLOAT && $3.type == FLOAT) {
            $$.type = FLOAT; 
            $$.value.floatValue = $1.value.floatValue + $3.value.floatValue; 
        }
    }
    | expr '-' expr
    {
        if ($1.type == INT && $3.type == INT) {
            $$.type = INT; 
            $$.value.intValue = $1.value.intValue - $3.value.intValue; 
        } else if ($1.type == FLOAT && $3.type == FLOAT) {
            $$.type = FLOAT; 
            $$.value.floatValue = $1.value.floatValue - $3.value.floatValue; 
        }
    }
    | expr '*' expr
    {
        if ($1.type == INT && $3.type == INT) {
            $$.type = INT; 
            $$.value.intValue = $1.value.intValue * $3.value.intValue; 
        } else if ($1.type == FLOAT && $3.type == FLOAT) {
            $$.type = FLOAT; 
            $$.value.floatValue = $1.value.floatValue * $3.value.floatValue; 
        }
    }
    | expr '/' expr
    { 
        if ($1.type == INT && $3.type == INT) {
            if ($3.value.intValue == 0) {
                yyerror("Error: can't divide by 0.");
                YYERROR;
            }
            $$.type = INT; 
            $$.value.intValue = $1.value.intValue / $3.value.intValue; 
        } else if ($1.type == FLOAT && $3.type == FLOAT) {
            if ($3.value.floatValue == 0) {
                yyerror("Error: can't divide by 0.");
                YYERROR;
            }
            $$.type = FLOAT; 
            $$.value.floatValue = $1.value.floatValue / $3.value.floatValue; 
        } 
    }
    | '(' expr ')' 
    { 
        if ($2.type == INT) {
            $$.type = INT;
            $$.value.intValue = $2.value.intValue; 
        } else if ($2.type == FLOAT) {
            $$.type = FLOAT; 
            $$.value.floatValue = $2.value.floatValue; 
        } 
        // else {
        //     yyerror("Error: can put this type in fact");
        //     YYERROR;
        // }
    }
    | '-' expr %prec MINUS_SIGNAL 
    { 
        if ($2.type == INT) {
            $$.type == INT;
            $$.value.intValue = - $2.value.intValue;
        } else if ($2.type == FLOAT) {
            $$.type == FLOAT;
            $$.value.floatValue = - $2.value.floatValue;
        } else {
            yyerror("Error: wrong type argument to unary minus");
            YYERROR;
        }
    }
    ;

type:
      TOK_INT
    | TOK_FLOAT   
    | TOK_STRING   
    | TOK_CHAR  

%%

void yyerror(const char *s) {
	extern int yylineno;    
	extern char * yytext;   
    printf("Erro: %s\nSímbolo: %s\nLinha: %d", s, yytext, yylineno);
}

int main(int argc, char **argv) {
    yyparse();
    return 0;
}
