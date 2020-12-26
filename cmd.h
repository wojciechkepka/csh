#ifndef CSH_CMDH
#define CSH_CMDH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CSH_BUILTIN_COUNT 4

extern char *USERHOME_p, *USERNAME_p;

void _csh_cd(char *path);
int csh_cd(char **args);
int csh_help(char **args);
int csh_exit(char **args);
int csh_tilde(char **args);
/**/
int csh_launch(char **args);
int csh_execute(char **args);

extern const char *builtin_commands[];

extern int (*builtin_funcs[]) (char **);

#endif // CSH_CMDH