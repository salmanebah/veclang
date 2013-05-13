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
extern  ast clone_ast(ast node);

#endif


