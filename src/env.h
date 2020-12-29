#ifndef CSH_ENVH
#define CSH_ENVH

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>


void csh_get_user_home(char *home);
void csh_get_username(char *name);

#endif // CSH_ENVH