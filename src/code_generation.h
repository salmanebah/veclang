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
