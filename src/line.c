#include "line.h"

const char *CLEAR_BACK_CHAR_SEQ = "\b\b\b   \b\b\b";
const char *ANSI_COL_RIGHT = "\033[1C";
const char *ANSI_COL_LEFT = "\033[1D";
const int DEL_KEY_FIRST = 0x33;
const int DEL_KEY_SECOND = 0x7e;
const int ARROW_UP = 0x41;
const int ARROW_DOWN = 0x42;
const int ARROW_RIGHT = 0x43;
const int ARROW_LEFT = 0x44;
const int HOME_KEY = 0x48;

char *csh_readline(void)
{
    size_t bufsize = CSH_INP_BUF_SIZE;
    char *buf = malloc(bufsize * sizeof(int));
    if (!buf)
    {
        fprintf(stderr, "Error: failed to allocate input buffer");
        exit(EXIT_FAILURE);
    }
    int ch;
    size_t pos = 0, max_pos = 0;
    bool was_escape = false, is_key_esc = false, print_ch, add_buf, is_del_key = false;

    csh_enable_raw_mode();
    while (true)
    {
        print_ch = true;
        add_buf = true;

        if (*GOT_CTRL_C_p)
        {
            *GOT_CTRL_C_p = 0;
            free(buf);
            return NULL;
        }

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
            csh_prompt_print();
            fflush(stdout);
            fflush(stdin);
            write(STDOUT_FILENO, buf, max_pos);
            continue;
        }

        if (ch == DEL)
        {
            if (pos > 0)
            {
                write(STDIN_FILENO, "\b\033[s\033[0K", 8);
                for(size_t c = pos - 1; c < max_pos; c++)
                {
                    buf[c] = buf[c+1];
                    write(STDIN_FILENO, &buf[c+1], 1);
                }
                write(STDIN_FILENO, "\033[u", 3);
                pos--;
                max_pos--;
            }
            
            continue;
        }

        // fprintf(stderr, "max = %d, esc = %d, arr = %d, ch = %.2x\n", max_pos, was_escape, is_arr, ch);
        
        if (is_del_key)
        {
            if (ch == DEL_KEY_SECOND)
            {
                is_del_key = false;
                if (pos < max_pos)
                {
                    write(STDIN_FILENO, "\033[s\033[0K", 8);
                    for(size_t c = pos; c < max_pos - 1; c++)
                    {
                        buf[c] = buf[c+1];
                        write(STDIN_FILENO, &buf[c+1], 1);
                    }
                    write(STDIN_FILENO, "\033[u", 3);
                    max_pos--;
                }
                continue;
            }
            else
            {
                // its not a del key
                write(STDIN_FILENO, "\033[3", 3);
                buf[pos++] = 0x1b;
                buf[pos++] = 0x5b;
                buf[pos++] = 0x33;
            }
        }

        

        if (is_key_esc)
        {
            is_key_esc = false;
            if (ch == ARROW_UP || ch == ARROW_DOWN || ch == ARROW_LEFT || ch == ARROW_RIGHT)
            {
                if (ch == ARROW_RIGHT)
                {
                    if (pos < max_pos)
                    {
                        write(STDIN_FILENO, ANSI_COL_RIGHT, 4);
                        pos++;
                    }
                }

                if (ch == ARROW_LEFT)
                {
                    if (pos > 0)
                    {
                        write(STDIN_FILENO, ANSI_COL_LEFT, 4);
                        pos--;
                    }
                }
                goto fix_pos;
                continue;
            }
            else if (ch == DEL_KEY_FIRST)
            {
                is_del_key = true;
                continue;
            }
            else if (ch == HOME_KEY)
            {
                char cols[256];
                sprintf(cols, "%ld", pos);
                write(STDIN_FILENO, "\033[", 2);
                write(STDIN_FILENO, cols, strlen(cols));
                write(STDIN_FILENO, "D", 1);
                pos = 0;
                continue;
            }
            else
            { // its not a key escape so write the escape sequence and [ to stdout
                write(STDIN_FILENO, "\033[", 2);
                buf[pos++] = 0x1b;
                buf[pos++] = 0x5b;
            }
        }

        if (was_escape && ch == 0x5b)
        {
            was_escape = false;
            is_key_esc = true;
            print_ch = false;
            add_buf = false;
        }

        if (was_escape && ch != 0x5b)
        { //its not an arrow so print \033 to stdout
            was_escape = false;
            write(STDIN_FILENO, "\033", 1);
            buf[pos++] = 0x1b;
        }

        if (ch == 0x1b)
        {
            was_escape = true;
            print_ch = false;
            add_buf = false;
        }

        if (ch == EOF || ch == '\n')
        {
            csh_disable_raw_mode();
            buf[max_pos] = '\0';
            fflush(stdin);
            fflush(stdout);
            write(STDIN_FILENO, "\n", 1);
            return buf;
        }

        if (add_buf) buf[pos++] = (char)ch;

        if (print_ch) write(STDIN_FILENO, &ch, 1);

        if (pos >= bufsize)
        {
            bufsize += CSH_INP_BUF_SIZE;
            buf = realloc(buf, bufsize);
            if (!buf)
            {
                fprintf(stderr, "Error: failed to allocate input buffer");
                exit(EXIT_FAILURE);
            }
        }

fix_pos:
        if (pos > max_pos) max_pos = pos;
    }
}

char **csh_split_line(char *line)
{
    size_t bufsize = CSH_TOK_BUF_SIZE, pos = 0;
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