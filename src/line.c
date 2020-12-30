#include "line.h"

const char *CLEAR_BACK_CHAR_SEQ = "\b\b\b   \b\b\b";
const char *ANSI_COL_RIGHT = "\033[1C";
const char *ANSI_COL_LEFT = "\033[1D";



int csh_readkey(void)
{
    ssize_t nread;
    char ch;
    while ((nread = read(STDIN_FILENO, &ch, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
        {
            perror("csh");
            exit(EXIT_FAILURE);
        }
    }

    if (ch == '\033')
    {
        char seq[5];

        if (read(STDIN_FILENO, &seq[0], 1) != 1) return ch;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return ch;


        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return ch;

                switch (seq[1])
                {
                    case '1':
                        if (seq[2] == '~') return HOME_KEY;

                        if (read(STDIN_FILENO, &seq[3], 1) != 1) return ch;
                        // if (seq[3] != '~') return ch; 

                        switch (seq[2])
                        {
                            // case '5': // F5
                            // case '7': // F6
                            // case '8': // F7
                            // case '9': // F8
                        }
                        break;
                    case '2':
                        if (read(STDIN_FILENO, &seq[3], 1) != 1) return ch;
                        switch (seq[2])
                        {
                            // case '0': // F9
                            // case '1': // F10
                            // case '3': // F11
                            // case '4': // F12
                        }
                        break;

                    case '3':
                        if (seq[2] == '~') return DEL_KEY;
                        break;
                    case '4':
                        if (seq[2] == '~') return END_KEY;
                        break;
                    case '5':
                        if (seq[2] == '~') return PAGE_UP;
                        break;
                    case '6':
                        if (seq[2] == '~') return PAGE_DOWN;
                        break;
                    case '7':
                        if (seq[2] == '~') return HOME_KEY;
                        break;
                    case '8':
                        if (seq[2] == '~') return END_KEY;
                        break;
                }
            }
            else
            {
                switch (seq[1])
                {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
            
        }
        else if (seq[0] == 'O')
        {
            switch (seq[1])
            {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }

        return DISABLED;
    }
    else
    {
        return ch;
    }
}

char *csh_readline(Csh *csh)
{
    size_t bufsize = CSH_INP_BUF_SIZE;
    char *buf = malloc(bufsize * sizeof(int));
    char cols[256];

    if (!buf)
    {
        fprintf(stderr, "Error: failed to allocate input buffer");
        exit(EXIT_FAILURE);
    }
    int key;
    size_t pos = 0, max_pos = 0;

    prompt_print(stdout, csh->prompt);
    csh_enable_raw_mode();
    while (true)
    {
        key = csh_readkey();
        switch (key)
        {
            case ARROW_DOWN:
            case ARROW_UP:
            case PAGE_UP:
            case PAGE_DOWN:
            case DISABLED:
                continue;

            case ARROW_LEFT:
                if (pos > 0)
                {
                    write(STDIN_FILENO, ANSI_COL_LEFT, 4);
                    pos--;
                }
                continue;

            case ARROW_RIGHT:
                if (pos < max_pos)
                {
                    write(STDIN_FILENO, ANSI_COL_RIGHT, 4);
                    pos++;
                }
                goto fix_pos;
                
            case HOME_KEY:
                if (pos > 0)
                {
                    sprintf(cols, "%ld", pos);
                    write(STDIN_FILENO, "\033[", 2);
                    write(STDIN_FILENO, cols, strlen(cols));
                    write(STDIN_FILENO, "D", 1);
                    pos = 0;
                }
                continue;
            
            case CTRL_E:
            case END_KEY:
                if (pos < max_pos)
                {
                    sprintf(cols, "%ld", max_pos - pos);
                    write(STDIN_FILENO, "\033[", 2);
                    write(STDIN_FILENO, cols, strlen(cols));
                    write(STDIN_FILENO, "C", 1);
                    pos = max_pos;
                }
                continue;

            case CTRL_L:
                csh_clear();
                prompt_print(stdout, csh->prompt);
                fflush(stdout);
                fflush(stdin);
                write(STDOUT_FILENO, buf, max_pos);
                continue;

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
            case CTRL_D:
                fflush(stdout);
                fflush(stdin);
                kill(getpid(), SIGUSR1);
# pragma GCC diagnostic push

            case BACKSPACE:
                if (pos > 0)
                {
                    write(STDOUT_FILENO, "\b\033[s\033[0K", 8);
                    for(size_t c = pos - 1; c < max_pos - 1; c++)
                    {
                        buf[c] = buf[c+1];
                        write(STDOUT_FILENO, &buf[c+1], 1);
                    }
                    write(STDOUT_FILENO, "\033[u", 3);
                    pos--;
                    max_pos--;
                }
                continue;

            case DEL_KEY:
                if (pos < max_pos)
                {
                    write(STDOUT_FILENO, "\033[s\033[0K", 8);
                    for(size_t c = pos; c < max_pos - 1; c++)
                    {
                        buf[c] = buf[c+1];
                        write(STDOUT_FILENO, &buf[c+1], 1);
                    }
                    write(STDOUT_FILENO, "\033[u", 3);
                    max_pos--;
                }
                continue;

            case EOF:
            case '\n':
                csh_disable_raw_mode();
                buf[max_pos] = '\0';
                fflush(stdin);
                fflush(stdout);
                write(STDOUT_FILENO, "\n", 1);
                return buf;
        }


        buf[pos++] = (char)key;
        write(STDOUT_FILENO, &key, 1);

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