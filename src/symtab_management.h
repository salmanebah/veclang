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
