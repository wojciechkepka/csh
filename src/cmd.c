#include "cmd.h"

const char *builtin_commands[] =
{
    "cd",
    "help",
    "exit",
    "~",
};

int (*builtin_funcs[]) (char **) =
{
    &csh_cd, 
    &csh_help, 
    &csh_exit, 
    &csh_tilde,
};


void _csh_cd(char *path)
{
    char *p;
    if (path[0] == '~')
    {
        p = csh_expand_tilde(path);
        if (p == NULL) {
            return;
        }
        if (chdir(p) != 0)
        {
            perror(p);
        }
        free(p);
    }
    else
    {
        if (chdir(path) != 0)
        {
            perror(path);
        }
    }
}

int csh_tilde(char **args)
{
    fprintf(stdout, "%s\n", USERHOME_p);
    _csh_cd(USERHOME_p);

    return 1;
}

int csh_cd(char **args)
{
    if (args[1] == NULL)
    {
        _csh_cd(USERHOME_p);
    }
    else
    {
        _csh_cd(args[1]);
    }
    return 1;
}

int csh_help(char **args)
{
    printf(
        "csh - yet another shell in C\n"
        "\n"
        "Available built-in commands:\n"
    );

    for (int i = 0; i < CSH_BUILTIN_COUNT; i++)
    {
        printf("\t- %s\n", builtin_commands[i]);
    }

    return 1;
}

int csh_exit(char **args)
{
    return 0;
}

int csh_launch(char **args)
{
    if (strcmp(args[0], "\0") == 0) return 1;

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) // child
    {
        if (execvp(args[0], args) == -1)
        {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0) // error
    {
        perror("csh");
    }
    else // parent
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int csh_execute(char **args)
{
    if (args[0] == NULL) return 1;

    for (int i = 0; i < CSH_BUILTIN_COUNT; i++)
    {
        if (strcmp(args[0], builtin_commands[i]) == 0)
        {
            return (builtin_funcs[i])(args);
        }
    }

    return csh_launch(args);
}