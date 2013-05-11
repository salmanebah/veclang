#ifndef  H_CODE_GENERATION_H
#define  H_CODE_GENERATION_H
#include "ast.h"
#include <stdio.h>
#include "queue.h"

/* veclang code to c based code */
extern void compile(ast root , char* output_name);
/* get equivalent c filename from veclang filename */
extern char* veclang_name_to_c_name(char* file);
/* traverses the abstract syntax tree  and replace cartesian expression nodes and polar nodes with simple cartesian nodes */
extern void first_pass (ast root);
/* replace cycle keyword with simple cartesian nodes */
extern void second_pass (queue fifo);
/* get the queue of all node commands in the ast */
extern void get_queue_command(ast root , queue fifo);
/* helper function , doing the recursive work */
extern void second_pass_aux(ast root , ast first_param , ast parent);
#endif
