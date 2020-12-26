/* csh - yet another shell in C
 *
 *
 * author: Wojciech Kępka <wojciech@wkepka.dev>
 */
#include <stdio.h>
#include <termios.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <pwd.h>
#include "cmd.h"

#define CSH_INP_BUF_SIZE 1024
#define CSH_TOK_BUF_SIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"

#define CTRL_C 0x03
#define DEL 0x07F
#define CTRL_L 0x0C
#define CTRL_D 0x04

const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
const char *CLEAR_BACK_CHAR_SEQ = "\b\b\b   \b\b\b";
const char *ANSI_COL_RIGHT = "\e[1C";
const char *ANSI_COL_LEFT = "\e[1D";
const char ARROW_UP[] = {0x1b, 0x5b, 0x41};
const char ARROW_DOWN[] = {0x1b, 0x5b, 0x42};
const char ARROW_RIGHT[] = {0x1b, 0x5b, 0x43};
const char ARROW_LEFT[] = {0x1b, 0x5b, 0x44};

static volatile int got_ctrl_c = 0;
static char CWD[PATH_MAX];
static char USERNAME[32];
static char USERHOME[PATH_MAX];
static int UID;
static struct termios ORIG_TERM_SETTINGS;

char *CWD_p = &(CWD[0]);
char *USERNAME_p = &(USERNAME[0]);
char *USERHOME_p = &(USERHOME[0]);
int *UID_p = &UID;


/* sets terminal back to original settings.
 */
void csh_disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ORIG_TERM_SETTINGS);
}

/* sets terminal into raw mode storing old settings in ORIG_TERM_SETTINGS.
 */
void csh_enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &ORIG_TERM_SETTINGS);
    atexit(csh_disable_raw_mode);

    struct termios raw = ORIG_TERM_SETTINGS;
    raw.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


/* sets value of UID to current uid of this process
 */
void csh_set_uid()
{
    UID = getuid();
}

/* aquires cwd of this process and stores it.
 */
void csh_set_cwd()
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
void csh_set_username()
{
    csh_get_username(USERNAME);
}

/* gets this process uid home directory and stores it in USERHOME var.
 */
void csh_set_user_home()
{
    csh_get_user_home(USERHOME);
}

/* updates username if the UID changed.
 */
void csh_set_username_if_changed()
{
    if (getuid() != UID)
    {
        csh_set_uid();
        csh_set_username();
    }
}

/* clears terminal screen by sending ANSI clear screen escape code.
 */
void csh_clear()
{
    write(STDIN_FILENO, CLEAR_SCREEN_ANSI, 12);
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
    int ch, position = 0, max_pos = 0;

    csh_enable_raw_mode();
    while (true)
    {
        ch = getchar();
        // printf("ch = `%c` %.x2\n", ch, ch);
        if (ch == CTRL_D)
        {
            fflush(stdout);
            fflush(stdin);
            free(buf);
            kill(getpid(), SIGUSR1);
        }

        if (ch == CTRL_L) // ctrl + l
        {
            csh_clear();
            return NULL;
        }

        if (ch == DEL)
        {
            if (position > 0)
            {
                write(STDIN_FILENO, "\b \b", 3);
                position -= 1;
            }
            
            continue;
        }

        if (ch == CTRL_C) return NULL;

        if (ch == ARROW_UP[2])
        {
            if (buf[position - 2] == ARROW_UP[0] && buf[position - 1] == ARROW_UP[1]) {
                position -= 3;
                continue;
            }
        }

        if (ch == ARROW_LEFT[2])
        {
            if (buf[position - 2] == ARROW_LEFT[0] && buf[position - 1] == ARROW_LEFT[1])
            {
                if (position > 0)
                {
                    write(STDIN_FILENO, ANSI_COL_LEFT, 3);
                    fflush(stdout);
                    fflush(stderr);
                }
                continue;
            }
        }

        if (ch == ARROW_RIGHT[2])
        {
            if (buf[position - 2] == ARROW_RIGHT[0] && buf[position - 1] == ARROW_RIGHT[1])
            {
                write(STDIN_FILENO, ANSI_COL_RIGHT, 3);
                if (position < max_pos) position++;
                continue;
            }
        }

        if (ch == EOF || ch == '\n')
        {
            csh_disable_raw_mode();
            buf[position] = '\0';
            putc('\n', stdout);
            return buf;
        }

        buf[position] = ch;
        putc(ch, stdout);
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

        if (position > max_pos) max_pos = position;
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
 * @returns always 1 to indicate another loop
 */
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
            return (builtin_funcs[i])(args);
        }
    }

    return csh_launch(args);
}

/* prints out prompt to stdout
 */
void csh_print_prompt()
{
    printf("%s@%s > ", USERNAME, CWD);
}

void csh_sigint_handler(int i) {
    got_ctrl_c = 1;
}

void csh_sigusr_handler(int i) {
    fprintf(stdout, "\nC you later :}");
    exit(EXIT_SUCCESS);
}

/* initializes static variables like uid, username and cwd
 */
void csh_init()
{
    signal(SIGINT, csh_sigint_handler);
    signal(SIGUSR1, csh_sigusr_handler);
    csh_set_uid();
    csh_set_username();
    csh_set_cwd();
    csh_set_user_home();
}

/* main loop of csh
 */
void csh_loop()
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
        csh_print_prompt();
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


int main()
{
    csh_loop();

    return EXIT_SUCCESS;
}
