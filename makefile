all: compiler

# Compiler
CPPC=gcc

# Lexer
FLEX=flex

# Yacc 
BISON=bison

compiler: lex.yy.c parser.tab.c
	$(CPPC) lex.yy.c parser.tab.c -o compiler

lex.yy.c: lex.l
	$(FLEX) lex.l

parser.tab.c: parser.y
	$(BISON) -d parser.y

clean:
	rm compiler lex.yy.c parser.tab.c parser.tab.h