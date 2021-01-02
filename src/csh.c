#include "csh.h"

/* local funcs */
static Csh *csh_init_env(Csh *csh);
static void csh_set_cwd(Csh *csh);
/**/

Csh *csh_new(void)
{
    Csh *csh = malloc(sizeof(Csh));
    if (!csh)
    {
        //err
        return NULL;
    }

    return csh_init_env(csh);
}

/* initializes fields of csh like username, current working directory or user home
 *
 * @returns a pointer to csh on success and null pointer on failure
 */
static Csh *csh_init_env(Csh *csh)
{
    csh_set_cwd(csh);
    csh_get_user_home(csh->userhome);
    if ((&csh->userhome) == NULL)
    {
        _ERRNO("failed to get user home directory");
    };
    csh->hist = history_init(CSH_HISTORY_CAPACITY);
    if (!csh->hist)
    {
        _ERRNO("failed to initialize command history");
    }
    csh_get_username(csh->username);
    if ((&csh->username) == NULL)
    {
        _ERRNO("failed to get user name");
    };
    csh->uid = getuid();

    csh->prompt = prompt_init(csh->username, csh->cwd);
    if (!csh->prompt)
    {
        return NULL;
    }

    return csh;
}

/* sets cwd field of csh to directory of this process. Usually used
 * through csh_init_env or csh_update
 *
 * @param csh a pointer to csh
 */
static void csh_set_cwd(Csh *csh)
{
    if (getcwd(csh->cwd, sizeof(csh->cwd)) == NULL)
    {
        _ERRNO("failed to get current working directory");
    }
}


void csh_update(Csh *csh)
{
    uid_t uid = getuid();
    if (uid != csh->uid)
    {
        if (csh_init_env(csh) == NULL)
        {
            //err
            _ERRNO("failed to update csh");
        }
    }
    else
    {
        csh_set_cwd(csh);
    }
    
}

void csh_free(Csh *csh)
{
    prompt_free(csh->prompt);
    history_free(csh->hist);
    free(csh);
}
