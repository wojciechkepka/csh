#include "env.h"

char *USERNAME_p = &(USERNAME[0]);
char *USERHOME_p = &(USERHOME[0]);
char *CWD_p = &(CWD[0]);
/* sets value of UID to current uid of this process
 */
void csh_set_uid(void)
{
    UID = getuid();
}

/* aquires cwd of this process and stores it.
 */
void csh_set_cwd(void)
{
    if (getcwd(CWD, sizeof(CWD)) == NULL)
    {
        fprintf(stderr, "failed to get current working directory\n");
    }
}

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

/* gets full username of this process uid and stores it in USERNAME var.
 */
void csh_set_username(void)
{
    csh_get_username(USERNAME);
}

/* gets this process uid home directory and stores it in USERHOME var.
 */
void csh_set_user_home(void)
{
    csh_get_user_home(USERHOME);
}

/* updates username if the UID changed.
 */
void csh_set_username_if_changed(void)
{
    if (getuid() != UID)
    {
        csh_set_uid();
        csh_set_username();
    }
}