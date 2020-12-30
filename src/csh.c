#include "csh.h"

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

Csh *csh_init_env(Csh *csh)
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

    csh->prompt = prompt_init(csh->username, csh->cwd);
    if (!csh->prompt)
    {
        return NULL;
    }

    return csh;
}

void csh_set_cwd(Csh *csh)
{
    if (getcwd(csh->cwd, sizeof(csh->cwd)) == NULL)
    {
        fprintf(stderr, "failed to get current working directory\n");
    }
}


void csh_update(Csh *csh)
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

void csh_free(Csh *csh)
{
    prompt_free(csh->prompt);
    history_free(csh->hist);
    free(csh);
}