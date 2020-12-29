#ifndef CSH_PROMPTH
#define CSH_PROMPTH

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"

#define F_USER 0x1
#define F_CWD 0x2
#define F_DELIM 0x4
#define F_ALL (F_USER | F_CWD | F_DELIM)


typedef struct {
    char *user, *cwd, *prompt;
    int flags, len;
} prompt_t;

/* prints out prompt to stdout
 */
prompt_t *csh_prompt_init(int flags, char *username, char *cwd);
void csh_prompt_free(prompt_t *p);
void csh_prompt_update(prompt_t *p);
void csh_prompt_print(prompt_t *p);

#endif // CSH_PROMPTH