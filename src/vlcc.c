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



