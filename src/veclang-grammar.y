/* Copyright (C) 2013 BAH Salmane, CHALAUX Benjamin , SOLLAUD Timothée

   This file is part of Veclang: a toy compiler for vectorial drawing using the cairo library
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
       #include "ast.h"
       #include "symtab_management.h"
       #include "scope_management.h"
       extern int yylineno;
       extern char* yytext;
       extern scope_tab global_scope;
%}


%union
{
     int integer_value;
     char* id_name;
     struct variable* var;
     struct ast_t* node;
     int v_type;
}
%parse-param {struct ast_t** root}

%token <node> DRAW FILL CYCLE LINE_WIDTH LINE_COLOR
%token <integer_value> NB 
%token TIRET "--"
%token AFFECT ":="
%token NEQ  "<>"
%token GEQ   ">="
%token LEQ   "<="
%token IMAGE IF ELSE
%token PATH SCAL PICT TRANSLATE ROTATE
%token <var> SCAL_ID PATH_ID PICT_ID
%token <id_name> ID

%type <v_type> type 
%type <node> list_of_instructions instruction command param /*assignment*/ image
%type <node> relative_param expr point test

%left "<=" ">=" "<>" '=' '<' '>'
%left '+' '-'
%left '*' '/'
%nonassoc ":="
%left NEG
%start program

%%


program : /* empty rule */ 
        | list_of_instructions                            {*root = $1;}
        ;

list_of_instructions : instruction ';'                        {$$ = $1;}
                     | list_of_instructions instruction ';'   {$$ = create_internal_node("list_of_instructions" , $1 , $2);}
                     ; 


instruction : command                     {$$ = $1;}
            | declaration                 {$$ = NULL;}
            | assignment                  {$$ = NULL;/*$$ = $1;*/}
            | test                        {$$ = $1;}
            ;

declaration : type list_of_id             

list_of_id : ID                             {add_symbol(global_scope , $1 , $<v_type>0);}                                 
           | list_of_id ',' ID              {add_symbol(global_scope , $3 , $<v_type>0);} 
           | SCAL_ID                        {add_symbol(global_scope , get_var_name($1) , $<v_type>0);}

           | PATH_ID                        {add_symbol(global_scope , get_var_name($1) , $<v_type>0);}
         
           | PICT_ID                        {add_symbol(global_scope , get_var_name($1) , $<v_type>0);}

           ;

assignment : PATH_ID ":=" param                   {if (check_in_scope(global_scope , get_var_name($1)) == true)
	                                               set_path_value($1 , $3);
                                                   else
						   {
							add_symbol(global_scope , get_var_name($1) , get_var_type($1));
							variable var = find_symbol(global_scope , get_var_name($1));
							set_path_value(var , $3);
						   }
                                             /*ast node = create_var_func_node(get_var_name($1));
                                             $$ = create_internal_node("path_assignment" , node , $3);*/}

           | SCAL_ID ":=" expr               {if (check_in_scope(global_scope , get_var_name($1)) == true)
	                                          set_scal_value($1 , eval_binary_ast($3));
                                             else
			               	      {
						   add_symbol(global_scope , get_var_name($1) , get_var_type($1));
						   variable var = find_symbol(global_scope , get_var_name($1));
						   set_scal_value(var , eval_binary_ast($3));
					      }
                                             /*ast node = create_var_func_node(get_var_name($1));
                                             $$ = create_internal_node("scal_assignment" , node , $3);*/}

           | PICT_ID ":=" image                    {if (check_in_scope(global_scope , get_var_name($1)) == true)
	                                               set_path_value($1 , $3);
                                                    else
						    {
							 add_symbol(global_scope , get_var_name($1) , get_var_type($1));
							 variable var = find_symbol(global_scope , get_var_name($1));
							 set_path_value(var , $3);
				                    }
                                              /*ast node = create_var_func_node(get_var_name($1));
                                              $$ = create_internal_node("pict_assignment" , node , $3);*/}
        
           | PATH_ID ":=" TRANSLATE '(' param ',' point ')'     {ast to_translate = create_internal_node("translate" , $5 , $7);
                                                                set_path_value($1 , apply_translation(to_translate));}

           | PATH_ID ":=" ROTATE '(' param ',' point ',' expr ')'     { ast argument = create_internal_node("argument" , $7 , $9);
		                                                       ast to_rotate = create_internal_node("rotate" , $5 , argument);
                                                                       set_path_value($1 , apply_rotation(to_rotate));}

           | PICT_ID ":=" TRANSLATE '(' image ',' point ')'     {ast to_translate = create_internal_node("translate" , $5 , $7);
                                                                set_path_value($1 , apply_translation(to_translate));}
           
           | PICT_ID ":=" ROTATE '(' image ',' point ',' expr ')'     { ast argument = create_internal_node("argument" , $7 , $9);
		                                                       ast to_rotate = create_internal_node("rotate" , $5 , argument);
                                                                       set_path_value($1 , apply_rotation(to_rotate));}

           
           | ID ":=" param  {printf("\033[31msemantic error\033[0m : variable used but not declared at line %d\n" , yylineno);
                             exit(EXIT_FAILURE);}
           | ID ":=" expr   {printf("\033[31msemantic error\033[0m : variable used but not declared at line %d\n" , yylineno);
                             exit(EXIT_FAILURE);}
           | ID ":="  TRANSLATE '(' param ',' point ')'  {printf("\033[31msemantic error\033[0m : variable used but not declared at line %d\n" , yylineno);
                                                         exit(EXIT_FAILURE);}

           ;


type : PATH             {$$ = PATH_TYPE;}  
     | SCAL             {$$ = SCAL_TYPE;}
     | PICT             {$$ = PICT_TYPE;}
     ;

image : IMAGE '{' list_of_instructions '}'        {
                                                   $$ = create_internal_node("image" , $3, NULL);
                                                  }

      | PICT_ID                                   {$$ = get_path_value($1);}

command : DRAW param                           {ast node = create_var_func_node("draw");
                                                $$ = create_internal_node("command" , node , $2);}
        | FILL param                           { ast node = create_var_func_node("fill");
                                                   $$ = create_internal_node("command" , node , $2);}
        | DRAW image                           {ast node = create_var_func_node("draw");
                                                $$ = create_internal_node("command" , node , $2);}

        | DRAW TRANSLATE '(' param ',' point ')'  {ast to_translate = create_internal_node("translate" , $4 , $6);
                                                  ast node = create_var_func_node("draw"); 
	                                          $$ = create_internal_node("command" , node , apply_translation(to_translate));}
    
        | DRAW ROTATE '(' param ',' point ',' expr ')'     { ast argument = create_internal_node("argument" , $6 , $8);
		                                             ast to_rotate = create_internal_node("rotate" , $4 , argument);
							     ast node = create_var_func_node("draw");
                                                             $$ = create_internal_node("command", node , apply_rotation(to_rotate));}


        | DRAW TRANSLATE '(' image ',' point ')'  {ast to_translate = create_internal_node("translate" , $4 , $6);

                                                   ast node = create_var_func_node("draw"); 
	                                           $$ = create_internal_node("command" , node , apply_translation(to_translate));}

        | DRAW ROTATE '(' image ',' point ',' expr ')'     { ast argument = create_internal_node("argument" , $6 , $8);
		                                             ast to_rotate = create_internal_node("rotate" , $4 , argument);
                                                             ast node = create_var_func_node("draw");
                                                             $$ = create_internal_node("command", node , apply_rotation(to_rotate));}



        | DRAW ID                              {printf("\033[31msemantic error\033[0m: variable used but not declared at line %d\n" , yylineno);
                                                exit(EXIT_FAILURE);}

	| FILL ID                              {printf("\033[31msemantic error\033[0m: variable used but not declared at line %d\n" , yylineno);
                                                exit(EXIT_FAILURE);}
        | LINE_WIDTH expr                     {ast node = create_var_func_node("line_width");
                                               $$ = create_internal_node("command", node , create_number_node(eval_binary_ast($2)));}

        | LINE_COLOR expr expr expr           {ast node = create_var_func_node("line_color");
                                               ast r = create_number_node(eval_binary_ast($2));
					       ast g = create_number_node(eval_binary_ast($3));
					       ast b = create_number_node(eval_binary_ast($4));
					       ast rgb = create_internal_node("color", r , create_internal_node("colo", g , b));
                                               $$ = create_internal_node("command", node , rgb);}
        ;

param : point relative_param                     {$$ = create_internal_node("param" , $1 , $2);}
      | PATH_ID relative_param                   {ast param = clone_ast(get_path_value($1));
                                                  merge_param(param , $2);
                                                  $$ = param;}
      ;

relative_param : /* empty rule */               {$$ = NULL;}
               | "--" '+' param                  {$$ = create_internal_node("relative_param_plus" , $3 , NULL);}
	       | "--" CYCLE relative_param       { ast node = create_var_func_node("cycle");
                                                  $$ = create_internal_node("relative_param_cycle" , node , $3);}
	       | "--" param                      {$$ = create_internal_node("relative_param" , $2 , NULL);}
               ;

point : '(' expr ',' expr ')'      {$$ = create_internal_node("cartesian_point" , $2 , $4);}
      | '(' expr ':' expr ')'       {$$ = create_internal_node("polar_point" , $2 , $4);}
      ;

expr : NB                          {$$ = create_number_node($1);}
     | SCAL_ID                     {$$ = create_number_node(get_scal_value($1));}
     | ID                          {printf("\033[31msemantic error\033[0m: variable used but not declared at line %d\n" , yylineno);
                                    exit(EXIT_FAILURE);}

     | expr '+' expr              {$$ = create_binary_node('+' , $1 , $3);}
     | expr '-' expr              {$$ = create_binary_node('-' , $1 , $3);}
     | '-' expr %prec NEG         {$$ = create_binary_node('-' , NULL , $2);}
     | expr '*' expr              {$$ = create_binary_node('*' , $1 , $3);}        
     | expr '/' expr              {$$ = create_binary_node('/' , $1 , $3);}        
     | '(' expr ')'               {$$ = $2;}
     | expr '<' expr              {$$ = create_binary_node('<', $1, $3);}
     | expr "<=" expr             {$$ = create_binary_node('l', $1, $3);}
     | expr '>' expr              {$$ = create_binary_node('>', $1, $3);}        
     | expr ">=" expr             {$$ = create_binary_node('m', $1, $3);}
     | expr '=' expr              {$$ = create_binary_node('=', $1, $3);}
     | expr "<>" expr             {$$ = create_binary_node('d' , $1 , $3);}
     ;



test :  IF '(' expr ')' '{' list_of_instructions '}' ELSE '{' list_of_instructions '}' 
                                              {if (eval_binary_ast($3))
							$$ = $6;
						else
						     $$ = $10;}

     | IF '(' expr ')' '{' list_of_instructions '}' 
                                                   {if (eval_binary_ast($3)) $$ = $6;
			                            else $$ = NULL;}
    ;




%%

void yyerror (const char* msg)
{
     fprintf(stderr , "%s line %d near token %s\n", msg , yylineno , yytext);
     exit(EXIT_FAILURE);
}                               /* warning yyerror */

#ifdef DEBUG_GRAMMAR
scope_tab global_scope;
int main(int argc, char *argv[])
{
     global_scope = create_scope_tab();
     struct ast_t* root;
     yyparse(&root);
     if (root != NULL)
	  print_ast(root , 0);
     ast_to_dot(root);
     return EXIT_SUCCESS;
}
#endif

