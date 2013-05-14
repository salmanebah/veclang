/* Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée/* 

/* This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
     Veclang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Veclang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Veclang.  If not, see <http://www.gnu.org/licenses/>
*/


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "veclang-lexer.h"
#include "veclang-grammar.h"
#include "ast.h"
#include "code_generation.h"
#include "scope_management.h"

scope_tab global_scope;
int main(int argc, char *argv[])
{
     ast root;
     global_scope = create_scope_tab();
     if (argc > 1)
	  for (int i = 1 ; i < argc ; ++i)
	  {
	       yyin = fopen(argv[i] , "r");
	       yyparse(&root);
	       char* output = veclang_name_to_c_name(argv[i]);
	       compile(root , output);
	       if (root != NULL)
		    free_ast(root);
	  }
     else
     {
	  yyparse(&root);
	  compile(root , "temp_gen.c");
	  free_ast(root);
     }
     printf("\033[01;34msuccessfully compiled\033[0m\n");
     return EXIT_SUCCESS;
}



