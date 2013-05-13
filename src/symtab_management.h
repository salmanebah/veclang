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

#ifndef H_SYMTAB_MANAGEMENT_H
#define H_SYMTAB_MANAGEMENT_H
#include "ast.h"
#define SCAL_TYPE 0
#define PATH_TYPE 1
#define PICT_TYPE 2
typedef struct symtab_t* symtab;
typedef struct variable* variable;




extern symtab create_symtable(void);
extern void free_symtable(symtab );
extern variable lookup(symtab , char*);
extern void install(symtab , char* ,  int);
extern int get_var_type(variable);
extern char* get_var_name(variable);
extern int get_scal_value(variable);
extern ast get_path_value(variable );
extern void set_scal_value (variable , int );
extern void set_path_value (variable , ast);
extern variable clone_variable(variable );
#endif
