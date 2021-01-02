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
} Prompt;

/* initializes a new prompt by dynamically allocating memory. It is the callers duty to free up
 * resources with prompt_free after use. The prompt is configurable by format string
 * read from CSH_FMT environment variable.
 * 
 * @param username name of the user to display
 * @param cwd current working directory to diplsay
 * @returns a pointer to Prompt
 */
Prompt *prompt_init(char *username, char *cwd);

/* frees up all resources used by a prompt
 *
 * @param p prompt to free
 */
void prompt_free(Prompt *p);

/* updates format string by checking if value of env variable CSH_FMT changed
 *
 * @param p a prompt to update
 * @returns 1 if something changed and 0 otherwise
 */
int prompt_update_fmt(Prompt *p);

/* sets the format string of this prompt
 *
 * @param p a prompt to edit
 * @param format an fmt string to set
 */
void prompt_set_fmt(Prompt *p, const char *format);

/* updates a prompt string in case one of the fields changed
 *
 * @params p prompt to update
 */
void prompt_update(Prompt *p);

/* prints a prompt to specified FILE
 *
 * @param f output file
 * @param p prompt to print
 */
void prompt_print(FILE *f, Prompt *p);

#endif // CSH_PROMPTH
