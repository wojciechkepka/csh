#ifndef CSH_ENVH
#define CSH_ENVH

#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static char CWD[PATH_MAX];
static char USERNAME[32];
static char USERHOME[PATH_MAX];
static __uid_t UID;
#pragma GCC diagnostic pop

extern char *USERNAME_p;
extern char *CWD_p;

void csh_set_uid(void);
void csh_set_cwd(void);
void csh_get_user_home(char *home);
void csh_get_username(char *name);
void csh_set_username(void);
void csh_set_user_home(void);
void csh_set_username_if_changed(void);


#endif // CSH_ENVH