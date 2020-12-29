#ifndef CSH_LINEH
#define CSH_LINEH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "term.h"
#include "prompt.h"
#include "csh.h"

#define CSH_INP_BUF_SIZE 1024
#define CSH_TOK_BUF_SIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"

#define CTRL_C 0x03
#define CTRL_E 0x05
#define CTRL_L 0x0C
#define CTRL_D 0x04
#define BACKSPACE 0x7F

enum key_e {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    DISABLED,
};

extern volatile int *GOT_CTRL_C_p;

/* reads characters from stdin, if escape sequence `\x1b` is encountered tries to read
 * the full escape sequence and interpret it as a key like ARROW_UP or PAGE_DOWN.
 * 
 * @returns read char or key sequence as key_e
 */
int csh_readkey(void);

/* reads from stdin character by character until EOF or '\n' is encountered. By default
 * allocates a buffer of CSH_INP_BUF_SIZE size and extends it as needed. It is the callers duty to
 * free allocated buffer after use.
 *
 * This function may quit the main process if there is an error with allocation or reallocation of
 * the input buffer.
 *
 * @arguments csh a pointer to a csh instance
 * @returns a line of input from stdin
 * */
char *csh_readline(csh_t *csh);

/* csh_split_line - splits input line into separate arguments by whitespace. This implementation is temporary and
 * will change into something more robust that handles "" and other nuances in the future.
 *
 * This function may quit the main process if there is an error with allocation or reallocation of
 * the token buffer.
 *
 * @param line a line of input to split up
 * @return parsed tokens
 */
char **csh_split_line(char *line);

#endif // CSH_LINEH