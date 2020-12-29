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
    size_t len;
} prompt_t;

/* initializes a new prompt by dynamically allocating memory. It is the callers duty to free up
 * resources with csh_prompt_free after use.
 * 
 * @param flags bitflags deciding which elements of prompt should be displayed. To disable
 * a certain flag pass for example `F_ALL & ~(F_USER)`
 * @param username name of the user to display
 * @param cwd current working directory to diplsay
 * @returns a pointer to prompt_t
 */
prompt_t *csh_prompt_init(int flags, char *username, char *cwd);

/* frees up all resources used by a prompt
 *
 * @param p prompt to free
 */
void csh_prompt_free(prompt_t *p);

/* updates a prompt string in case one of the fields changed
 *
 * @params p prompt to update
 */
void csh_prompt_update(prompt_t *p);

/* prints a prompt to stdout
 *
 * @param p prmopt to print
 */
void csh_prompt_print(prompt_t *p);

#endif // CSH_PROMPTH