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

#define _BSD_SOURCE
#include "symtab_management.h"
#include <assert.h>
#include <string.h>
#include "ast.h"
#include <stdbool.h>
#include <stdio.h>

static void free_variable(variable);

struct variable
{
     char* name;
     bool init;
     int  type;
     union
     {
	  int scal_value;
	  ast path_value;
     };
     
     variable next;
};

struct symtab_t
{
     variable peek;
};

symtab create_symtable(void)
{
     symtab this = (symtab)malloc(sizeof(struct symtab_t));
     assert(this != NULL);
     this-> peek = NULL;
     return this;
}

void free_symtable(symtab self)
{
     variable current ;
     variable next_current;
     if (self-> peek == NULL)
	  free(self);
     else
     {
	  for (current = self-> peek , next_current = current-> next;
	       next_current != NULL ; 
	       current = next_current , next_current = next_current-> next)
	       free(current);
	  free(self);
     }
}

variable lookup(symtab this , char* name)
{
     variable iterator;
     for(iterator = this-> peek ; iterator != NULL ; iterator = iterator-> next)
	  if (strcmp(iterator-> name , name) == 0)
	       return iterator;
     return NULL;

}

void install(symtab this , char* name ,  int type)
{
     variable var = (variable)malloc(sizeof(struct variable));
     assert(var != NULL);
     var-> type = type;
     var-> init = false;
     var-> name = strdup(name);
     var-> next = this-> peek;
     this->peek = var;
}

int get_scal_value (variable var)
{
     assert(var != NULL && var-> type == SCAL_TYPE);
     if (var-> init == false)
     {
	  printf("\033[31msemantic error\033[0m: variable %s used but not assign\n" , var-> name);
	  exit(EXIT_FAILURE);
     }
     return var-> scal_value;
}

ast get_path_value (variable var)
{
     assert(var != NULL && (var-> type == PATH_TYPE || var-> type == PICT_TYPE));
     if (var-> init == false)
     {
	  printf("\033[31msemantic error\033[0m: variable %s used but not assign\n" , var-> name);
	  exit(EXIT_FAILURE);
     }
     return var-> path_value;
}

void set_scal_value(variable var , int val)
{
     assert(var != NULL);
     var-> init = true;
     var-> scal_value = val;
}
void set_path_value(variable var , ast root)
{
     assert(var != NULL);
     var-> init = true;
     var-> path_value = root;
}

void free_variable(variable var)
{
     assert (var != NULL);
     free(var-> name);
     if (var-> type == PATH_TYPE || var-> type == PICT_TYPE)
	  free_ast(var-> path_value);
     free(var);
}

int get_var_type(variable var)
{
     return var-> type;
}

char* get_var_name(variable var)
{
     return var-> name;
}

variable clone_var (variable src)
{
     variable var = (variable)malloc(sizeof(struct variable));
     assert(var != NULL);
     var-> type = get_var_type(src);
     var-> name = get_var_name(src);
     if (get_var_type(src) == SCAL_TYPE)
	  set_scal_value(var, get_scal_value(src));
     else
	  set_path_value(var, get_path_value(src));
     return var;
}
