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
    if ((&csh->userhome) == NULL)
    {
        fprintf(stderr, "Failed to get user home directory: %s\n", strerror(errno));
    };
    csh->hist = history_init(CSH_HISTORY_CAPACITY);
    if (!csh->hist)
    {
        // no history   
        fprintf(stderr, "Failed to initialize History: %s\n", strerror(errno));
    }
    csh_get_username(csh->username);
    if ((&csh->username) == NULL)
    {
        fprintf(stderr, "Failed to get user name: %s\n", strerror(errno));
    };
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
        fprintf(stderr, "Failed to get current working directory: %s\n", strerror(errno));
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
            fprintf(stderr, "Failed to update csh: %s\n", strerror(errno));
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