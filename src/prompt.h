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
    int flags;
} prompt_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static prompt_t *PROMPT;
#pragma GCC diagnostic pop

/* prints out prompt to stdout
 */
void csh_prompt_print(void);
void csh_prompt_init(int flags);
void csh_prompt_free(void);
void csh_prompt_update(void);

#endif // CSH_PROMPTH