#include "line.h"

const char *CLEAR_BACK_CHAR_SEQ = "\b\b\b   \b\b\b";
const char *ANSI_COL_RIGHT = "\e[1C";
const char *ANSI_COL_LEFT = "\e[1D";
const char ARROW_UP[] = {0x1b, 0x5b, 0x41};
const char ARROW_DOWN[] = {0x1b, 0x5b, 0x42};
const char ARROW_RIGHT[] = {0x1b, 0x5b, 0x43};
const char ARROW_LEFT[] = {0x1b, 0x5b, 0x44};

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