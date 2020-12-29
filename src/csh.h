#ifndef CSH_CSHH
#define CSH_CSHH

#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"
#include "env.h"

typedef struct
{
    char cwd[PATH_MAX];
    char username[32];
    char userhome[PATH_MAX];
    __uid_t uid;
    prompt_t *prompt;
} csh_t;

csh_t *csh_new(void);
csh_t *csh_init_env(csh_t *csh);
void csh_update(csh_t *csh);
void csh_set_cwd(csh_t *csh);
void *csh_free(csh_t *csh);


#endif // CSH_CSHH