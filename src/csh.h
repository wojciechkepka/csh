#ifndef CSH_CSHH
#define CSH_CSHH

#include <limits.h>
#include <unistd.h>
#include "prompt.h"
#include "env.h"
#include "history.h"
#include "defs.h"

#define CSH_HISTORY_CAPACITY 512

/* wrapper structure groupping all widely used elements together
 */
typedef struct
{
    char cwd[PATH_MAX];
    char username[32];
    char userhome[PATH_MAX];
    uid_t uid;
    Prompt *prompt;
    History *hist;
} Csh;

/* initializes a new instance of csh
 *
 * @returns a pointer to csh or a null pointer if there was an error with allocation
 */
Csh *csh_new(void);

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
