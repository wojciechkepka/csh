#include "csh.h"

csh_t *csh_new(void)
{
    csh_t *csh = malloc(sizeof(csh_t));
    if (!csh)
    {
        //err
        return NULL;
    }

    return csh_init_env(csh);
}

csh_t *csh_init_env(csh_t *csh)
{
    csh_set_cwd(csh);
    csh_get_user_home(csh->userhome);
    csh->hist = history_init(CSH_HISTORY_CAPACITY);
    if (!csh->hist)
    {
        // no history   
    }
    csh_get_username(csh->username);
    csh->uid = getuid();

    csh->prompt = csh_prompt_init(F_ALL, csh->username, csh->cwd);
    if (!csh->prompt)
    {
        return NULL;
    }

    return csh;
}

void csh_set_cwd(csh_t *csh)
{
    if (getcwd(csh->cwd, sizeof(csh->cwd)) == NULL)
    {
        fprintf(stderr, "failed to get current working directory\n");
    }
}


void csh_update(csh_t *csh)
{
    __uid_t uid = getuid();
    if (uid != csh->uid)
    {
        if (csh_init_env(csh) == NULL)
        {
            //err
        }
    }
    else
    {
        csh_set_cwd(csh);
    }
    
}

void csh_free(csh_t *csh)
{
    csh_prompt_free(csh->prompt);
    free(csh);
}