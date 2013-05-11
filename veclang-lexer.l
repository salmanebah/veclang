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

%}

%x COMMENT_MODE
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

"START"                { DO_DEBUG;
                         return START;}

"FINISH"               { DO_DEBUG;
                         return FINISH;}

"@<"                   { BEGIN(COMMENT_MODE);}


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
