#include "scope_management.h"
#include <assert.h>
#include <stdio.h>

typedef struct symtab_cell* symtab_cell;

static symtab_cell create_new_symtab_cell(void);
static void free_symtab_cell(symtab_cell);

struct symtab_cell
{
     symtab record;
     symtab_cell next;
};

struct scope_tab_t
{
     symtab_cell current;
};

scope_tab create_scope_tab(void)
{
     scope_tab self = (scope_tab)malloc(sizeof(struct scope_tab_t));
     assert(self != NULL);
     self-> current = create_new_symtab_cell();
     return self;
}

void enter_new_scope(scope_tab self)
{
     symtab_cell tab = create_new_symtab_cell();
     tab-> next = self-> current;
     self-> current = tab;
}

void exit_scope (scope_tab self)
{
     symtab_cell to_free = self-> current;
     self-> current = to_free-> next;
     free_symtab_cell(to_free);
}

void add_symbol(scope_tab self , char* name,  int type)
{
     symtab on_top = self-> current-> record;
     if (check_in_scope(self , name) == true)
     {
	  printf("\033[31msemantic error\033[0m :variable %s already defined in current scope\n" , name);
	  exit(EXIT_FAILURE);
     }
     else
	  install(on_top , name , type);
}

variable find_symbol (scope_tab self , char* name)
{
     symtab_cell on_top = self-> current;
     variable var = NULL;
     while (on_top != NULL)
     {
	  var = lookup(on_top-> record , name);
	  if (var != NULL)
	       return var;
	  on_top = on_top-> next;
     }
     return NULL;
}

bool check_in_scope (scope_tab self , char* name)
{
     symtab on_top = self-> current-> record;
     variable var = lookup(on_top , name);
     if (var != NULL)
	  return true;
     return false;
}

symtab_cell create_new_symtab_cell(void)
{
     symtab_cell self = (symtab_cell)malloc(sizeof(struct symtab_cell));
     assert(self != NULL);
     self-> record = create_symtable();
     self-> next = NULL;
	  
}

void free_symtab_cell(symtab_cell self)
{
     free_symtable(self-> record);
     free(self);
}

#ifdef DEBUG_SCOPE_MANAGEMENT
int main(int argc, char *argv[])
{
     scope_tab global = create_scope_tab();
     add_symbol(global , "var" , SCAL_TYPE);
     add_symbol(global , "var" , PATH_TYPE);
     printf("outer scope var: %p\n" , find_symbol(global , "var"));
     enter_new_scope(global);
     add_symbol(global , "i" , SCAL_TYPE);
     add_symbol(global , "var" , PATH_TYPE);
     add_symbol(global , "j" , SCAL_TYPE);
     variable v = find_symbol(global , "var");
     printf("inner scope var: %p\n" , v);
     v = find_symbol(global , "i");
     printf("var i : %p\n" , v);
     enter_new_scope(global);
     v = find_symbol(global , "j");
     printf("var j : %p\n" , v);
     exit_scope(global);
     v = find_symbol(global, "j");
     printf("var j : %p\n" , v);
     exit_scope(global);
     v = find_symbol(global , "var");
     printf("outer scope var : %p\n" , v);
     exit_scope(global);
     return EXIT_SUCCESS;
}

#endif
     
