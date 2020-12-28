/* csh - yet another shell in C
 *
 *
 * author: Wojciech Kępka <wojciech@wkepka.dev>
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cmd.h"
#include "line.h"
#include "prompt.h"
#include "env.h"

#pragma GCC diagnostic ignored "-Wmissing-prototypes"

static volatile int got_ctrl_c = 0;

volatile int *GOT_CTRL_C_p = &got_ctrl_c;


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
