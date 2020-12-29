#ifndef CSH_PROMPTH
#define CSH_PROMPTH

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "env.h"

#define CSH_FORMAT_ENV "CSH_FMT"
#define CSH_DEFAULT_PROMPT "%u@%d > "

typedef struct {
    char *user, *cwd, *prompt, *format;
    size_t len;
} prompt_t;

/* initializes a new prompt by dynamically allocating memory. It is the callers duty to free up
 * resources with csh_prompt_free after use. The prompt is configurable by format string
 * read from CSH_FMT environment variable.
 * 
 * @param username name of the user to display
 * @param cwd current working directory to diplsay
 * @returns a pointer to prompt_t
 */
prompt_t *csh_prompt_init(char *username, char *cwd);

/* frees up all resources used by a prompt
 *
 * @param p prompt to free
 */
void csh_prompt_free(prompt_t *p);


size_t csh_prompt_calculate_len(prompt_t *p);

void csh_prompt_update_fmt(prompt_t *p);
void csh_prompt_set_fmt(prompt_t *p, const char *format);

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