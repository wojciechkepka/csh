#include "cmd.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"


const char *builtin_commands[] =
{
    "cd",
    "help",
    "exit",
    "~",
    "export",
    "history",
};

int (*builtin_funcs[]) (csh_t *, char **) =
{
    &csh_cd, 
    &csh_help, 
    &csh_exit, 
    &csh_tilde,
    &csh_export,
    &csh_history,
};


void _csh_cd(csh_t *csh, char *path)
{
    char *p;
    if (path[0] == '~')
    {
        p = csh_expand_tilde(csh->userhome, path);
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

int csh_tilde(csh_t *csh, char **args)
{
    fprintf(stdout, "%s\n", csh->userhome);
    _csh_cd(csh, csh->userhome);

    return 1;
}

int csh_cd(csh_t *csh, char **args)
{
    if (args[1] == NULL)
    {
        _csh_cd(csh, csh->userhome);
    }
    else
    {
        _csh_cd(csh, args[1]);
    }
    return 1;
}

int csh_help(csh_t *csh, char **args)
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

int csh_exit(csh_t *csh, char **args)
{
    return 0;
}

int csh_export(csh_t *csh, char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "missing variable to export. Example `FOO=bar`\n");
    }
    else
    {
        const char *sep = "=";
        char *p = strpbrk(args[1], sep);
        if (p != NULL)
        {
            size_t len = (size_t)(p - args[1]);
            char *name = malloc((len + 1) * sizeof(char));
            if (!name)
            {
                fprintf(stderr, "failed to allocate memory for env var");
            }
            else
            {
                strncpy(name, args[1], len);
                setenv(name, ++p, 1);
                free(name);
            }
        }
    }

    return 1;
}

int csh_history(csh_t *csh, char **args)
{
    for (size_t i = 0; i <= csh->hist->back - 1; i++)
    {
        fprintf(stdout, "\t%ld\t%s\n", i, csh->hist->elems[i]);
    }

    return 1;
}

/* */

int csh_launch(char **args)
{
    if (strcmp(args[0], "\0") == 0) return 1;

    pid_t pid;
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
            waitpid(pid, &status, WUNTRACED);
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int csh_execute(csh_t *csh, char **args)
{
    if (args[0] == NULL) return 1;

    for (int i = 0; i < CSH_BUILTIN_COUNT; i++)
    {
        if (strcmp(args[0], builtin_commands[i]) == 0)
        {
            return (builtin_funcs[i])(csh, args);
        }
    }

    return csh_launch(args);
}