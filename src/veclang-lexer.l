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

%{
     #include <stdlib.h>
     #include <stdio.h>
     #include "veclang-grammar.h"
     #include "symtab_management.h"
     #include "scope_management.h"
     extern scope_tab global_scope;
     int debug_flag = 0;
     #define DO_DEBUG  if (debug_flag == 1)			\
	                        printf("%s " , yytext)
     int erreur_lexicale(char* message)
     {
	  fprintf(stderr,"lexical error on line %d : %s (unknown token)\n", yylineno , message);
	  exit(EXIT_FAILURE);
     }
     #define MAX_EXECUTE_LEVEL 20
     YY_BUFFER_STATE execute_stack[MAX_EXECUTE_LEVEL];
     int execute_stack_pointer = 0;
     char filename[30] = {'\0'};
     FILE* in;
     FILE* default_in;
     int i = 0;
%}

SPACE [[:blank:]]*
%x COMMENT_MODE
%x EXECUTE_MODE
%option yylineno

%%


"draw"                 {  DO_DEBUG;
	                  return DRAW;}

"fill"                 { DO_DEBUG;
                         return FILL;}

"cycle"                { DO_DEBUG;
	                 return CYCLE;}


"line_width"           { DO_DEBUG;
	                  return LINE_WIDTH;}

"line_color"           { DO_DEBUG;
	                  return LINE_COLOR;}

"translate"             { DO_DEBUG;
	                  return TRANSLATE;}

"rotate"                { DO_DEBUG;
	                  return ROTATE;}

"image"                 { DO_DEBUG;
                         return IMAGE;}
"pict"                 { DO_DEBUG;
                         return PICT;}

"--"                   { DO_DEBUG;
                         return TIRET;}

"path"                 { DO_DEBUG;
                         return PATH;}


"scal"                 { DO_DEBUG;
                         return SCAL;} 

"if"                  { DO_DEBUG;
                         return IF;}

"else"                 { DO_DEBUG;
                         return ELSE;}


"@<"                   { BEGIN(COMMENT_MODE);}

^{SPACE}"execute"{SPACE}\"                 { i = 0;
                                             BEGIN(EXECUTE_MODE);}


":="                   { DO_DEBUG;
                         return AFFECT;}

"<="                   { DO_DEBUG;
                         return LEQ;}

">="                   { DO_DEBUG;
                         return GEQ;}

"<>"                   { DO_DEBUG;
                         return NEQ;}

"{"                    { DO_DEBUG;
                         enter_new_scope(global_scope);
                         return yytext[0];
                       }

"}"                    { DO_DEBUG;
                         exit_scope(global_scope);
                         return yytext[0];
                       }

";" |
"+" |
"(" |
")" |
"," |
":" |
"-" |
">" |
"<" |
"=" |
"*" |
"/"                    {DO_DEBUG;
	                return yytext[0];}


[[:digit:]]+               {DO_DEBUG;
                            yylval.integer_value = atoi(yytext);
                            return NB;}

[[:alpha:]_][[:alnum:]]*     {DO_DEBUG;
                              variable var = find_symbol(global_scope , yytext);
			      if (var != NULL)
			      {
				   if (get_var_type(var) == SCAL_TYPE)
                                       {
                                         yylval.var = var;
                                         return SCAL_ID;
                                       }
				   else if (get_var_type(var) == PATH_TYPE)
                                     {
                                       yylval.var = var;
                                       return PATH_ID;
                                     }
                                    else
                                       yylval.var = var;
                                       return PICT_ID;
                               } 
			      yylval.id_name = yytext;
                              return ID; 
                             }
                            

[[:blank:]\n]                         { /* ignore */}

.  {erreur_lexicale(yytext);}

<COMMENT_MODE>
{
     ">@"            { BEGIN(INITIAL);}
     
     .|\n            { /* ignore */} 
}

<EXECUTE_MODE>
{
     [^\"\n\t]          {filename[i++] = yytext[0];}

     \"                 { printf("filename get: %s\n" , filename);
	                  in = fopen(filename , "r");
			       if (in != NULL)
			       {
				       printf("filename get and read\n");
				       enter_new_scope(global_scope);
				       execute_stack[execute_stack_pointer++] = YY_CURRENT_BUFFER;
				       yy_switch_to_buffer(yy_create_buffer(in , YY_BUF_SIZE));
				       BEGIN(INITIAL);
			      	}
			       else
			       {
					 printf("error while opening stream\n");
					 yyterminate();
				}
                                     
                            }

     \n                       {erreur_lexicale("newline not allowed in load directive\n");}
     

}

<<EOF>>                         { if (--execute_stack_pointer < 0)
	                            yyterminate();
                                  else
				  {
				       exit_scope(global_scope);
				       yy_delete_buffer(YY_CURRENT_BUFFER);
				       yy_switch_to_buffer(execute_stack[execute_stack_pointer]);
				  }
      
                                    
                                 }
%%

#ifdef DEBUG_LEXER
int main(int argc, char *argv[])
{
     debug_flag = 1;
     do
     {
	  yylex();
     }while (!feof (yyin));
	  
     return EXIT_SUCCESS;
}
#endif
