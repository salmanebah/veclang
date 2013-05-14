# Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée/* 

#  This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
#    Veclang is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.

#  Veclang is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with Veclang.  If not, see <http://www.gnu.org/licenses/>

CC = gcc
this ?= demo
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
	./$< $(this).vl
	$(CC) $(this).c `pkg-config --cflags --libs cairo` -o $(this)
	./$(this)
	evince output.pdf

.PHONY: clean distclean
clean:
	rm -f veclang-lexer.c
	rm -f veclang-grammar.c
	rm -f veclang-grammar.h
	rm -f veclang-lexer.h
	rm -f veclang-grammar.output
	rm -f veclang-grammar.dot
	rm -f *.o*
distclean:
	rm -f $(EXEC)
	rm -f debug-grammar
	rm -f debug-lexer
	rm -f debug-ast
