#include "path.h"

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