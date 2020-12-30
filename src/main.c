/* csh - yet another shell in C
 *
 *
 * author: Wojciech Kępka <wojciech@wkepka.dev>
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cmd.h"
#include "line.h"
#include "prompt.h"
#include "env.h"
#include "csh.h"
#include "history.h"

#pragma GCC diagnostic ignored "-Wmissing-prototypes"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void _sigint_handler(int _) {}

void _sigusr_handler(int _) 
{
    printf("\nC you later :}\n");
    exit(EXIT_SUCCESS);
}
#pragma GCC diagnostic pop

/* initializes static variables like uid, username and cwd
 */
void _init_signals(void)
{
    signal(SIGINT, _sigint_handler);
    signal(SIGUSR1, _sigusr_handler);
}

/* main loop of csh
 */
void csh_loop(void)
{
    _init_signals();

    Csh *csh = csh_new();
    if (!csh)
    {
        fprintf(stderr, "failed to create csh");
        exit(EXIT_FAILURE);
    }

    char *line;
    char **args;
    int status;

    do
    {
        csh_update(csh);
        line = csh_readline(csh);
        if (strcmp(line, "") == 0 || line == NULL)
        {
            status = 1;
            free(line);
            continue;
        }

        history_add(csh->hist, line);
        
        args = csh_split_line(line);
        status = csh_execute(csh, args);

        free(args);
        free(line);
    }
    while (status);

    csh_free(csh);
}


int main(void)
{
    csh_loop();

    return EXIT_SUCCESS;
}
