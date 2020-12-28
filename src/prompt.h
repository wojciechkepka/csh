#ifndef CSH_PROMPTH
#define CSH_PROMPTH

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define F_USER 0x1
#define F_CWD 0x2
#define F_DELIM 0x4
#define F_ALL (F_USER | F_CWD | F_DELIM)

extern char *USERNAME_p, *CWD_p;


typedef struct {
    char *user, *cwd, *prompt;
    int flags;
} prompt_t;

static prompt_t *PROMPT;

/* prints out prompt to stdout
 */
void csh_prompt_print(void);
void csh_prompt_init(int flags);
void csh_prompt_free(void);
void csh_prompt_update(void);

#endif // CSH_PROMPTH