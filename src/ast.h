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


#ifndef H_AST_H
#define H_AST_H
#include <stdlib.h>

typedef struct ast_t* ast;

typedef enum 
{
     NUMBER,
     VARIABLE,
     BINARY_OP,
     FUNCTION,
     INTERNAL

} node_type;

struct ast_t
{
     node_type type;
     union
     {
	  char* node_name;
	  int node_value;
	  char operation;
     };
     ast right;
     ast left;
	  
     
};

/* create an internal node (e.g : command , param) */
extern ast create_internal_node(char* node_type , ast left , ast right);
/* create a binary node  */
extern ast create_binary_node (char operation , ast left , ast right);
/* create number node (terminal) */
extern ast create_number_node(int val);
/* create node for variable and function */
extern ast create_var_func_node( char* name);
/* return the type of the node */
extern int  get_node_type(ast node);
extern int is_leaf(ast node);
/* free a node */
extern void free_ast (ast node);
/* show nodes of the tree */
extern void print_ast(ast node , int padding);
/* evaluate a node */
extern int eval_binary_ast(ast node);
//extern void tree_to_dot(ast node);
extern void merge_param(ast param , ast relative_param);
extern ast apply_translation(ast node);
extern ast apply_rotation(ast node);
/* deep copy of the given node */
extern  ast clone_ast(ast node);
/* convert ast to dot */
extern void ast_to_dot(ast node);
#endif


