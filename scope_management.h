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
