#include "env.h"


void csh_get_user_home(char *home)
{
    struct passwd *u;
    errno = 0;
    u = getpwuid(getuid());
    if (u == NULL)
    {
        fprintf(stderr, "Error: failed to get passwd database entry of current user.");
        home = NULL;
    }
    else
    {
        strcpy(home, u->pw_dir);
    }
}

void csh_get_username(char *name)
{
    struct passwd *u;
    errno = 0;
    u = getpwuid(getuid());
    if (u == NULL)
    {
        fprintf(stderr, "Error: failed to get passwd database entry of current user.");
        name = NULL;
    }
    else
    {
        strcpy(name, u->pw_name);
    }
}

char *csh_expand_tilde(char *home, char *path)
{
    char *expanded = malloc((strlen(home) + strlen(path)) * sizeof(char));
    if (!expanded)
    {
        fprintf(stderr, "failed to allocate memory to expand path");
        return NULL;
    }
    expanded[0] = '\0';
    strcat(expanded, home);
    strcat(expanded, path + 1); // skip ~ 

    return expanded;
}