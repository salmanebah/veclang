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



/*TODO: eval binary tree during it creation */
#include "ast.h"
#include <assert.h>
#include <string.h>
#include <math.h>
#include "code_generation.h"
#define RADIAN 0.0174532925
static void apply_translation_aux(ast param , int dx , int dy);
static void apply_rotation_aux(ast param , int cx , int cy , float angle);
static char* ast_to_dot_aux (FILE* out , ast node);
/* give a unique name to a node for dot */
static char* different_node_name(char* node_name , int i);

static void sim_mark(int n)
{
     int i;
     for( i = 0 ; i < n ; ++i)
	  printf("|");
}
ast create_internal_node( char* node_name , ast left , ast right)
{
     ast self = (ast)malloc (sizeof(struct ast_t));
     assert(self != NULL);
     self-> type = INTERNAL;
     self-> node_name = (char*)malloc (sizeof (char) * (strlen(node_name) + 1));
     assert(self-> node_name != NULL);
     strcpy(self-> node_name, node_name);
     self-> right = right;
     self-> left = left;
     return self;
}

ast create_binary_node (char operation , ast left , ast right)
{
     ast self = (ast) malloc(sizeof (struct ast_t));
     assert(self != NULL);
     self-> type = BINARY_OP;
     self-> operation = operation;
     self-> right = right;
     self-> left = left;
     return self;
}
ast create_number_node (int val)
{
     ast self = (ast) malloc(sizeof (struct ast_t));
     assert(self != NULL);
     self-> type = NUMBER;
     self-> node_value = val;
     self-> right = self-> left = NULL;
     return self;
}

ast create_var_func_node(char* node_name)
{
     ast self = (ast) malloc(sizeof (struct ast_t));
     assert(self != NULL);
     if ((strcmp(node_name , "draw") == 0) || (strcmp(node_name , "fill") == 0)
	 || (strcmp(node_name , "cycle") == 0))
	  self-> type = FUNCTION;
     else
	  self-> type = VARIABLE;
     self-> node_name = (char*)malloc (sizeof (char) * (strlen(node_name) + 1));
     assert(self-> node_name != NULL);
     strcpy(self-> node_name , node_name);
     self-> right = self-> left = NULL;
     return self;
     
}

int eval_binary_ast(ast node)
{
     /* TODO:add for var later */
     if (node-> type == NUMBER)
	  return node-> node_value;
     else
     {
	  switch(node-> operation)
	  {
	  case '+':
	       return eval_binary_ast(node-> left) + eval_binary_ast(node-> right);

	  case '*' :
	       return eval_binary_ast(node-> left) * eval_binary_ast(node-> right);
	       
	  case '-':
	       return eval_binary_ast(node-> left) - eval_binary_ast(node-> right);

	  case '/':
	       return eval_binary_ast(node-> left) / eval_binary_ast(node-> right);

	  case '<':
	       return eval_binary_ast(node-> left) < eval_binary_ast(node-> right);

	  case 'l' :
	       return eval_binary_ast(node-> left) <= eval_binary_ast(node-> right);
	       
	  case '>':
	       return eval_binary_ast(node-> left) > eval_binary_ast(node-> right);

	  case 'm':
	       return eval_binary_ast(node-> left) >= eval_binary_ast(node-> right);

	  case '=':
	       return eval_binary_ast(node-> left) == eval_binary_ast(node-> right);

	  case 'd' :
	       return eval_binary_ast(node-> left) != eval_binary_ast(node-> right);

	  default:
	       printf("error while node evaluation");
	       exit(EXIT_FAILURE);
	  }
     }
}

void free_ast (ast node)
{
     if (is_leaf(node))
	  free(node);
     else
     {
     	  if (node-> right != NULL)
	       free_ast(node-> right);
     	  if (node-> left != NULL)
	       free_ast(node-> left);
	  free(node);
     }

}

int is_leaf (ast node)
{
     return (node-> right == NULL && node-> left == NULL);
}

int  get_node_type(ast node)
{
     return node-> type;
}

void print_ast (ast node , int padding)
{
     if (!is_leaf (node))
     {
	  if (node-> type == BINARY_OP)
	  {
	       sim_mark(padding);
	       printf("%c\n" , node-> operation);
	  }
	  else if (node-> type == INTERNAL  || node-> type == FUNCTION)
	  {
	       sim_mark(padding);
	       printf("%s\n" , node-> node_name);
	  }
	  if (node-> left != NULL)
	       print_ast(node-> left , padding + 1);
	  if (node-> right != NULL)
	       print_ast(node-> right , padding + 1);
     }
     else
     {
	  if (node-> type == NUMBER)
	       printf("%d\n" , node-> node_value);
	  else
	       printf("%s\n" , node-> node_name);
     }
	  
}

void ast_to_dot (ast node)
{
     FILE* out = fopen("ast.dot" , "w+");
     fprintf(out, "digraph G {\n");
     ast_to_dot_aux(out , node);
     fprintf(out , "\n}");
     fclose(out);
}

     
char* ast_to_dot_aux(FILE* out , ast node)
{
     static int i = 0;
     i++;
     if (node == NULL)
	  return different_node_name("NULL" ,i);
     char* left;
     char* right;
     char* name;
     switch(node-> type)
     {
     case INTERNAL :
	  name = different_node_name(node-> node_name,i);
	  left = ast_to_dot_aux(out , node-> left);
	  right = ast_to_dot_aux(out , node-> right);
	  fprintf(out,"%s -> %s;\n" , name , left);
	  fprintf(out , "%s -> %s;\n" , name , right);
	  return name;
     case NUMBER :
	  return different_node_name("NUMBER" , i);
     case FUNCTION :
	  return different_node_name(node-> node_name , i);
     case BINARY_OP :
	  name = different_node_name("BINOP" , i);
	  left = ast_to_dot_aux(out , node-> left);
	  right = ast_to_dot_aux(out , node-> right);
	  fprintf(out , "BINOP -> %s;\n" , name , left);
	  fprintf(out, "BINOP -> %s;\n" , name , right);
	  return name;
     }
}
char* different_node_name(char* node_name , int i)
{
     char* to_return = (char*)malloc(sizeof(strlen(node_name)) + 2);
     assert(to_return != NULL);
     sprintf(to_return , "%s_%d" , node_name , i);
     return to_return;

}
 
void merge_param (ast param , ast relative_param)
{
     assert(param != NULL);
     ast merge_node = param;
     while(merge_node-> right != NULL)
	  merge_node = merge_node-> right;
     merge_node-> right = relative_param;

	  
}
ast clone_ast(ast node)
{
     static int i = 0;
     if (node == NULL)
	  return;
     ast clone;
     ast left , right;
     node_type type = node-> type;
     switch(type)
     {
     case INTERNAL :
	  left = clone_ast(node-> left);
	  right = clone_ast(node-> right);
	  clone = create_internal_node(node->node_name, left , right);
	  return clone;
     case NUMBER:
	  clone = create_number_node(node-> node_value);
	  return clone;
     case BINARY_OP :
	  left = clone_ast(node-> left);
	  right = clone_ast(node-> right);
	  clone = create_binary_node(node-> operation , left , right);
	  return clone;
     case FUNCTION :
	  clone = create_var_func_node(node->node_name);
	  return clone;
     }
	  
     
}


ast apply_translation(ast node)
{
     ast cloned_node = clone_ast(node);
     first_pass(cloned_node);
     ast to_translate = cloned_node-> left;
     if (strcmp(to_translate-> node_name , "param") == 0)
     {
	  ast first_param = clone_ast(to_translate-> left);
	  second_pass_aux(to_translate-> right , first_param , to_translate);
     }
     else
     {
	  queue fifo;
	  fifo = queue_create();
	  get_queue_command(cloned_node-> left , fifo);
	  second_pass(fifo);
	  queue_destroy(fifo);
	  
     }
     int dx = cloned_node-> right-> left-> node_value;
     int dy = cloned_node-> right-> right-> node_value;
     apply_translation_aux(to_translate , dx , dy);
     return cloned_node-> left;
}

ast apply_rotation(ast node)
{
     ast cloned_node = clone_ast(node);
     first_pass(cloned_node);
     ast to_rotate = cloned_node-> left;
     if (strcmp(to_rotate-> node_name , "param") == 0)
     {
	  ast first_param = clone_ast(to_rotate-> left);
	  second_pass_aux(to_rotate-> right , first_param , to_rotate);
     }
     else
     {
	  queue fifo;
	  fifo = queue_create();
	  get_queue_command(cloned_node-> left , fifo);
	  second_pass(fifo);
	  queue_destroy(fifo);
	  
     }
     ast argument = cloned_node-> right;
     int cx = argument-> left-> left-> node_value;
     int cy = argument-> left-> right-> node_value;
     float angle = argument-> right-> node_value * RADIAN;
     apply_rotation_aux(to_rotate , cx , cy , angle);
     return cloned_node-> left;
}

void apply_rotation_aux(ast param , int cx , int cy , float angle)
{
     if (param == NULL)
	  return;
     else if (strcmp(param-> node_name , "cartesian_point") == 0)
     {
	  int px = param-> left-> node_value;
	  int py = param-> right-> node_value;
	  int ppx = ceil(cos(angle) * (px - cx) - sin(angle) * (py - cy) + cx);
	  int ppy = ceil(sin(angle) * (px - cx) + cos(angle) * (py - cy) + cy);
	  param-> left-> node_value = ppx;
	  param-> right-> node_value = ppy;
     }
     else
     {
	  apply_rotation_aux(param-> left , cx , cy , angle);
	  apply_rotation_aux(param-> right , cx , cy , angle);
     }

}

void apply_translation_aux(ast param , int dx , int dy)
{
     if (param == NULL)
	  return;
     else if (strcmp(param-> node_name , "cartesian_point") == 0)
     {
	  param-> left-> node_value += dx;
	  param-> right-> node_value += dy;
     }
     else
     {
	  apply_translation_aux(param-> left , dx , dy);
	  apply_translation_aux(param-> right, dx , dy);
     }
     
}
#ifdef DEBUG_AST

int main(int argc, char *argv[])
{
     ast left = create_number_node(40);
     ast right = create_var_func_node("draw");
     ast root = create_internal_node("program" , right, left);
     ast_to_dot(root);
     free_ast(root);
     return EXIT_SUCCESS;
}

#endif
