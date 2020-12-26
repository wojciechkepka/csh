#include "path.h"

char *csh_expand_tilde(char *path)
{
    char *expanded = malloc(sizeof(*USERHOME_p) + sizeof(path));
    if (!expanded)
    {
        fprintf(stderr, "failed to allocate memory to expand path");
        return NULL;
    }

    strcat(expanded, USERHOME_p);
    strcat(expanded, path + 1); // skip ~ 

    return expanded;
}