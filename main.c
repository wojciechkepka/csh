/* csh - yet another shell in C
 *
 *
 * author: Wojciech Kępka <wojciech@wkepka.dev>
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define CSH_INP_BUF_SIZE 1024
#define CSH_TOK_BUF_SIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"
#define CSH_BUILTIN_COUNT (sizeof(builtin_commands) / sizeof(char *))

static volatile int got_ctrl_c = 0;

int csh_cd(char **args);
int csh_help(char **args);
int csh_exit(char **args);

char *builtin_commands[] =
{
    "cd",
    "help",
    "exit",
};

int (*builtin_funcs[]) (char **) =
{
    &csh_cd, 
    &csh_help, 
    &csh_exit, 
};

/* builtin implementation of cd command.
 *
 * @param args arguments to call cd with.
 */
int csh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "cd: expected argument\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("csh");
        }
    }
    return 1;
}

/* builtin implementation of help command.
 *
 * @param args arguments to call help with.
 */
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

/* builtin implementation of exit command.
 *
 * @param args arguments to call exit with.
 */
int csh_exit(char **args)
{
    return 0;
}

/* csh_readline - reads from stdin character by character until EOF or '\n' is encountered. By default
 * allocates a buffer of CSH_INP_BUF_SIZE size and extends it as needed. It is the callers duty to
 * free allocated buffer after use.
 *
 * This function may quit the main process if there is an error with allocation or reallocation of
 * the input buffer.
 *
 * @returns a line of input from stdin
 * */
char *csh_readline()
{
    int bufsize = CSH_INP_BUF_SIZE;
    char *buf = malloc(bufsize * sizeof(int));
    
    if (!buf)
    {
        fprintf(stderr, "Error: failed to allocate input buffer");
        exit(EXIT_FAILURE);
    }

    int ch, position = 0;
    while (true)
    {
        ch = getchar();

        if (got_ctrl_c == 1)
        {
            return buf;
        }

        if (ch == EOF || ch == '\n')
        {
            buf[position] = '\0';
            return buf;
        }

        buf[position] = ch;
        position++;

        if (position >= bufsize)
        {
            bufsize += CSH_INP_BUF_SIZE;
            buf = realloc(buf, bufsize);
            if (!buf)
            {
                fprintf(stderr, "Error: failed to allocate input buffer");
                exit(EXIT_FAILURE);
            }
        }
    }
}

/* csh_split_line - splits input line into separate arguments by whitespace. This implementation is temporary and
 * will change into something more robust that handles "" and other nuances in the future.
 *
 * This function may quit the main process if there is an error with allocation or reallocation of
 * the token buffer.
 *
 * @param line a line of input to split up
 * @return parsed tokens
 */
char **csh_split_line(char *line)
{
    int bufsize = CSH_TOK_BUF_SIZE, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "Error: failed to allocate token buffer");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, CSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[pos] = token;
        pos++;

        if (pos >= bufsize)
        {
            bufsize += CSH_TOK_BUF_SIZE;
            tokens = realloc(tokens, bufsize);

            if (!tokens)
            {
                fprintf(stderr, "Error: failed to allocate token buffer");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, CSH_TOK_DELIM);
    }
    tokens[pos] = NULL;

    return tokens;
}

/* launches specified command command with args as child process.
 *
 * @param args - an array with executable name as first element and arguments as rest
 * @returns always 1
 */
int csh_launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) // child
    {
        if (execvp(args[0], args) == -1)
        {
            perror("csh");
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
        while (!WIFEXITED(status) && ! WIFSIGNALED(status));
    }

    return 1;
}

/* executes arguments parsed from a line of input. This function first searches for matches
 * in builtin commands. If no match is found args are executed as a command.
 *
 * @param args - command with arguments to run
 */
int csh_execute(char **args)
{
    if (args[0] == NULL) return 1;

    for (int i = 0; i < CSH_BUILTIN_COUNT; i++)
    {
        if (strcmp(args[0], builtin_commands[i]) == 0)
        {
            return (*builtin_funcs[i])(args);
        }
    }

    return csh_launch(args);
}

/* main loop of csh
 */
void csh_loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("> ");
        line = csh_readline();
        
        if (got_ctrl_c == 1) {
            got_ctrl_c = 0;
            goto cleanup;
        }

        args = csh_split_line(line);
        status = csh_execute(args);

cleanup:
        free(line);
        free(args);
    }
    while (status);
}

void csh_sigint_handler(int i) {
    got_ctrl_c = 1;
    printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
    char c;
     c = getchar();
}

int main()
{
    signal(SIGINT, csh_sigint_handler);
    csh_loop();

    return EXIT_SUCCESS;
}
