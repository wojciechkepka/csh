/* csh - yet another shell in C
 *
 *
 * author: Wojciech Kępka <wojciech@wkepka.dev>
 */
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <pwd.h>
#include "cmd.h"
#include "line.h"
#include "prompt.h"

#pragma GCC diagnostic ignored "-Wmissing-prototypes"

static volatile int got_ctrl_c = 0;
static char CWD[PATH_MAX];
static char USERNAME[32];
static char USERHOME[PATH_MAX];
static __uid_t UID;

char *CWD_p = &(CWD[0]);
char *USERNAME_p = &(USERNAME[0]);
char *USERHOME_p = &(USERHOME[0]);
__uid_t *UID_p = &UID;
volatile int *GOT_CTRL_C_p = &got_ctrl_c;


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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void csh_sigint_handler(int _) {
    got_ctrl_c = 1;
}

void csh_sigusr_handler(int _) {
    printf("\nC you later :}\n");
    exit(EXIT_SUCCESS);
}
#pragma GCC diagnostic pop //-Wunused-parameters

/* initializes static variables like uid, username and cwd
 */
void csh_init(void)
{
    signal(SIGINT, csh_sigint_handler);
    signal(SIGUSR1, csh_sigusr_handler);
    csh_set_uid();
    csh_set_username();
    csh_set_cwd();
    csh_set_user_home();
    csh_prompt_init(F_ALL);
    atexit(csh_prompt_free);
}

/* main loop of csh
 */
void csh_loop(void)
{
    csh_init();

    char *line;
    char **args;
    int status;

    do
    {
        csh_set_uid();
        csh_set_username_if_changed();
        csh_set_cwd();
        csh_prompt_print();
        line = csh_readline();
        if (line == NULL)
        {
            continue;
        }
        
        args = csh_split_line(line);
        status = csh_execute(args);

        free(args);
        free(line);
    }
    while (status);
}


int main(void)
{
    csh_loop();
    return EXIT_SUCCESS;
}
