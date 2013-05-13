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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>
*/

#define _BSD_SOURCE
#include "code_generation.h"
#include <math.h>
#include <string.h>
#define RADIAN 0.0174532925

/* produce the headers and initialize cairo context */
static void initialize_code_gen(FILE* file);
/* finalize cairo context */
static void finalize_code_gen(FILE* file);
/* produce code from ast */
static void ast_to_code_gen(queue fifo , FILE* file);
/* get the first param of a command node (i.e fill | draw) */
static ast get_first_param(ast command_node);
/* evaluate complex binary operation */
static void binary_expr_to_number(ast root , ast parent);
/* convert polar node to cartesian node */
static void polar_node_to_cartesian(ast root);
/* convert cycle node to cartesian */
static void cycle_to_point(ast root ,  ast first_param , ast parent);
/* convert non-trivial cartesian node to simple cartesian node */
static  void cartesian_expr_to_simple(ast root);
/* helper function, doing the recursive work */
static void ast_to_code_aux(ast param , FILE* file);


void compile (ast root , char* output_name)
{
     if (root == NULL)
	  return;
     queue fifo = queue_create();
     queue lilo = queue_create();
     FILE* out = fopen(output_name , "w+");
     initialize_code_gen(out);
     get_queue_command(root , fifo);
     get_queue_command(root , lilo);
     first_pass(root);
     second_pass(fifo);
     ast_to_code_gen(lilo , out);
     finalize_code_gen(out);
     queue_destroy(fifo);
     queue_destroy(lilo);
     
}

char* veclang_name_to_c_name(char* file)
{
     int len = strlen(file) - 1;
     char* c_name = strndup(file , sizeof(char) * len);
     c_name[len - 1] = 'c';
     return c_name;
}
void binary_expr_to_number(ast root , ast parent)
{
     int val = eval_binary_ast(root);
     ast node = create_number_node(val);
     if (root == parent-> left)
	  parent-> left = node;
     else
	  parent-> right = node;
     free_ast(root);
     //   return;
}
void cartesian_expr_to_simple(ast root)
{
     binary_expr_to_number(root-> left , root);
     binary_expr_to_number(root-> right , root);
//     return;
}

void polar_node_to_cartesian(ast root)
{
     cartesian_expr_to_simple(root);
     int rayon = root-> right-> node_value;
     int angle = root-> left-> node_value;
     int x = ceil(rayon * cos(angle * RADIAN));
     int y = ceil(rayon * sin(angle * RADIAN));
     ast node_x = create_number_node(x);
     ast node_y = create_number_node(y);
     root-> left = node_x;
     root-> right = node_y;
     strcpy(root-> node_name , "cartesian_point");
}

void cycle_to_point(ast root , ast first_param , ast parent)
{
     parent-> left = first_param;
     free_ast(root);
     return;
     
}

void initialize_code_gen(FILE* file)
{
     const char* init = "/* Automatically generated code */\n"
	               "/* PLEASE DON'T MODIFY THIS , UNLESS YOU KNOW WHAT YOU ARE DOING :) */\n"
	               "\n"
                         "#include <stdlib.h>\n"
                         "#include  <cairo.h>\n"
                         "#include <cairo-pdf.h>\n"
			 "\n"
	                 "int main(int argc , char** argv )\n"
                          "{\n"
	                     "\tcairo_surface_t *surface;\n"
	                      "\tcairo_t *cr;\n"
	                      "\tcairo_surface_t* pdf_surface = cairo_pdf_surface_create(\"output.pdf\" , 600 ,600);\n"
	                      "\tcr=cairo_create(pdf_surface);\n";
     fprintf(file , "%s" , init);
	  
}

void finalize_code_gen(FILE* file)
{
     const char* final = "\tcairo_destroy(cr);\n"
                         "\tcairo_surface_destroy(pdf_surface);\n"
	                 "\treturn EXIT_SUCCESS;\n"
                         "}\n";

     fprintf(file , "%s" , final);
     fclose(file);
}

ast get_first_param(ast command_node)
{
     return clone_ast(command_node-> right->left);
}
void first_pass (ast root)
{
          if (root != NULL)
	  {
	       if (root-> type == INTERNAL)
	       {
		    if (strcmp(root-> node_name , "cartesian_point") == 0)
			 cartesian_expr_to_simple(root);
		    else if (strcmp(root-> node_name , "polar_point") == 0)
			 polar_node_to_cartesian(root);
		    first_pass(root-> left);
		    first_pass(root-> right);
	       }
	  }
 }

void second_pass (queue fifo)
{
     while (!queue_empty(fifo))
     {
	  ast command_node = queue_front(fifo);
	  ast first_param = get_first_param(command_node);
	  ast parent_arg  = command_node-> right;
	  ast first_arg = parent_arg-> right;
	  second_pass_aux(first_arg , first_param , parent_arg);
	  queue_pop(fifo);

     }
}

void second_pass_aux(ast root , ast first_param , ast parent)
{
     if (root != NULL)
     {
	  if (root-> type == FUNCTION)
	       if (strcmp(root-> node_name , "cycle") == 0)
		    cycle_to_point(root , first_param , parent);
	  second_pass_aux(root-> left , first_param , root);
	  second_pass_aux(root-> right , first_param , root);
     }
}

void get_queue_command(ast root , queue fifo)
{
        if (root-> type == INTERNAL) 
	{ 
 	  if (strcmp(root-> node_name , "command") == 0) 
	       queue_push(fifo , root);
	  if (root-> left != NULL) 
	       get_queue_command(root-> left , fifo); 
 	  if (root-> right != NULL)
 	       get_queue_command(root-> right , fifo);
	}
}

void ast_to_code_gen( queue fifo , FILE* file)
{
     ast current = queue_front(fifo);
     char* function = current-> left-> node_name;
     ast move_param = current-> right-> left;
     if (strcmp(function , "line_width") == 0)
     {
	  fprintf(file, "\tcairo_set_line_width (cr, %d);\n", current-> right-> node_value);
	  queue_pop(fifo);
	  ast_to_code_gen(fifo, file);
     }
     else if (strcmp(function , "line_color") == 0)
     {
	  float r = move_param-> node_value/100.0;
	  float g = current-> right-> right-> left-> node_value/100.0;
	  float b = current-> right-> right-> right-> node_value/100.0;
	  fprintf(file, "\tcairo_set_source_rgb (cr, %f, %f, %f);\n", r , g , b);
	  queue_pop(fifo);
	  ast_to_code_gen(fifo, file);
     }
     else
     {
	  if (strcmp(current-> right-> node_name , "image") == 0)
	  {
	       queue_pop(fifo);
	       ast_to_code_gen(fifo, file);
	  }
     
	  else
	  {
	       int x = move_param-> left-> node_value;
	       int y = move_param-> right-> node_value;
	       fprintf(file , "\tcairo_move_to(cr , %d , %d);\n" , x , y);
	       if (current-> right-> right != NULL)
		    ast_to_code_aux(current-> right-> right, file);
	       if (strcmp(function , "draw") == 0)
		    fprintf(file , "\tcairo_stroke(cr);\n");
	       else
		    fprintf(file , "\tcairo_fill(cr);\n");
	       queue_pop(fifo);
	       if (!queue_empty(fifo))
		    ast_to_code_gen(fifo, file);
	  }
     }
}

void ast_to_code_aux (ast param , FILE* file)
{
     if (strcmp(param-> node_name , "relative_param_plus") == 0)
     {
	  int x = param-> left-> left-> left-> node_value;
	  int y = param-> left-> left-> right-> node_value;
	  fprintf(file , "\tcairo_rel_line_to(cr , %d , %d);\n" , x , y);
	  if (param-> left-> right != NULL)
	       ast_to_code_aux(param-> left-> right , file);
	  if (param-> right != NULL)
	       ast_to_code_aux(param->right , file);
     }
     else if (strcmp(param-> node_name , "relative_param_cycle") == 0)
     {
	  int x = param-> left-> left-> node_value;
	  int y = param-> left-> right-> node_value;
	  fprintf(file , "\tcairo_line_to(cr , %d , %d);\n" , x , y);
	  if (param-> right != NULL)
	  ast_to_code_aux(param-> right , file);
     }
     else
     {
	  int x = param-> left-> left-> left-> node_value;
	  int y = param-> left-> left-> right-> node_value;
	  fprintf(file , "\tcairo_line_to(cr , %d , %d);\n" , x , y);
	  if (param-> left-> right != NULL)
	       ast_to_code_aux(param-> left-> right , file);
	  if (param-> right != NULL)
	       ast_to_code_aux(param-> right, file);
     }
     
}
