#ifndef CSH_CSHH
#define CSH_CSHH

#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"
#include "env.h"
#include "history.h"

#define CSH_HISTORY_CAPACITY 512

/* wrapper structure groupping all widely used elements together
 */
typedef struct
{
    char cwd[PATH_MAX];
    char username[32];
    char userhome[PATH_MAX];
    __uid_t uid;
    Prompt *prompt;
    History *hist;
} Csh;

/* initializes a new instance of csh
 *
 * @returns a pointer to csh or a null pointer if there was an error with allocation
 */
Csh *csh_new(void);

/* initializes fields of csh like username, current working directory or user home
 *
 * @returns a pointer to csh on success and null pointer on failure
 */
Csh *csh_init_env(Csh *csh);

/* sets cwd field of csh to directory of this process. Usually used
 * through csh_init_env or csh_update
 *
 * @param csh a pointer to csh
 */
void csh_set_cwd(Csh *csh);

/* udates all fields of csh if uid changed otherwise only updates cwd
 *
 * @param csh a pointer to csh
 */
void csh_update(Csh *csh);

/* frees up all memory used by this structure
 *
 * @param csh a pointer to csh
 */
void csh_free(Csh *csh);


#endif // CSH_CSHH