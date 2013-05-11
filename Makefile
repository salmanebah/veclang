CC = gcc
CFLAGS = -g -std=c99 -w
LDFLAGS = -lfl -lm
FLEX = flex
BISON = bison
HEADER = $(wildcard *.h)
OBJ = veclang-grammar.o veclang-lexer.o ast.o vlcc.o code_generation.o queue_list.o symtab_management.o scope_management.o
EXEC = vlcc
check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

all: $(EXEC)

$(EXEC): $(OBJ) 
	$(CC) $^ -o $@ $(LDFLAGS) 

%.o: %.c  $(HEADER)
	$(CC) -c $< -o $@ $(CFLAGS)

veclang-grammar.c : veclang-grammar.y
	$(BISON) -v -g  --debug -d  -o $@ $<

veclang-lexer.c : veclang-lexer.l symtab_management.c scope_management.c
	$(FLEX) --header-file=$*.h -o $@ $<

debug-lexer : veclang-lexer.c veclang-lexer.h 
	$(CC) $< -o debug-lexer $(LDFLAGS) -DDEBUG_LEXER

debug-grammar : veclang-grammar.c veclang-lexer.c ast.c symtab_management.c scope_management.c queue_list.c code_generation.c
	$(CC) $^ -o  $@ $(LDFLAGS) -DDEBUG_GRAMMAR

debug-scope_management : scope_management.c symtab_management.c ast.c
	$(CC) $^ -o  $@ $(LDFLAGS) -DDEBUG_SCOPE_MANAGEMENT

debug-ast : ast.c
	$(CC) $< -o $@ $(CFLAGS) -DDEBUG_AST


draw : $(EXEC)
	./$< demo.vl
	$(CC) demo.c `pkg-config --cflags --libs cairo` -o demo
	./demo
	evince output.pdf

.PHONY: clean distclean
clean:
	rm -f veclang-lexer.c
	rm -f veclang-grammar.c
	rm -f *.o*
distclean:
	rm -f $(EXEC)
	rm -f debug-grammar
	rm -f debug-lexer
	rm -f debug-ast
