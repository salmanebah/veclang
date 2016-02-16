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

#ifndef H_SCOPE_MANAGEMENT_H
#define H_SCOPE_MANAGEMENT_H
#include "symtab_management.h"
#include <stdbool.h>

typedef struct scope_tab_t* scope_tab;

extern scope_tab create_scope_tab(void);
extern void enter_new_scope(scope_tab );
extern void exit_scope(scope_tab );
extern void add_symbol(scope_tab , char* , int );
extern variable find_symbol(scope_tab , char* );
extern bool check_in_scope(scope_tab , char*);

#endif
