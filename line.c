#include "line.h"

const char *CLEAR_BACK_CHAR_SEQ = "\b\b\b   \b\b\b";
const char *ANSI_COL_RIGHT = "\e[1C";
const char *ANSI_COL_LEFT = "\e[1D";
const int ARROW_UP = 0x41;
const int ARROW_DOWN = 0x42;
const int ARROW_RIGHT = 0x43;
const int ARROW_LEFT = 0x44;

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
        // fprintf(stderr ,"ch = `%c` %.2x\n", ch, ch);
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
            csh_print_prompt();
            fflush(stdout);
            write(STDOUT_FILENO, buf, max_pos);
            continue;
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

        if (buf[position - 2] == 0x1b && buf[position - 1] == 0x5b)
        {
            if (ch == ARROW_UP || ch == ARROW_DOWN || ch == ARROW_LEFT || ch == ARROW_RIGHT)
            {
                position -= 2;
                buf[position] = '\0';
                max_pos = position;

                if (ch == ARROW_RIGHT)
                {
                    write(STDIN_FILENO, ANSI_COL_RIGHT, 3);
                    if (position < max_pos) position++;
                }

                if (ch == ARROW_LEFT)
                {
                    if (position > 0)
                    {
                        write(STDIN_FILENO, ANSI_COL_LEFT, 3);
                        fflush(stdout);
                        fflush(stderr);
                    }
                }

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