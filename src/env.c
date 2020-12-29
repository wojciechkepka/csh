#include "env.h"

/* gets this process uid home directory.
 *
 * On error prints the error to stdout and assigns NULL to passed in pointer.
 */
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

/* gets this process uid username.
 *
 * On error prints the error to stdout and assigns NULL to passed in pointer.
 */
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
