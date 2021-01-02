#ifndef CSH_LINEH
#define CSH_LINEH

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
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

enum EKey {
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static struct termios ORIG_TERM_SETTINGS;
#pragma GCC diagnostic pop

/* sets terminal back to original settings.
 */
void csh_disable_raw_mode(void);

/* sets terminal into raw mode storing old settings in ORIG_TERM_SETTINGS.
 */
void csh_enable_raw_mode(void);

/* clears terminal screen by sending ANSI clear screen escape code.
 */
void csh_clear(void);

/* reads characters from stdin, if escape sequence `\x1b` is encountered tries to read
 * the full escape sequence and interpret it as a key like ARROW_UP or PAGE_DOWN.
 * 
 * On error returns -2 and sets errno to appropriate value.
 * 
 * @returns read char or key sequence as EKey
 */
int csh_readkey(void);

/* reads from stdin character by character until EOF or '\n' is encountered. By default
 * allocates a buffer of CSH_INP_BUF_SIZE size and extends it as needed. It is the callers duty to
 * free allocated buffer after use.
 *
 * On error returns a null pointer and sets errno to appropriate value.
 *
 * @arguments csh a pointer to a csh instance
 * @returns a line of input from stdin
 * */
char *csh_readline(Csh *csh);

/* csh_split_line - splits input line into separate arguments by whitespace. This implementation is temporary and
 * will change into something more robust that handles "" and other nuances in the future.
 *
 * On error returns a null pointer and sets errno to appropriate value.
 *
 * @param line a line of input to split up
 * @return parsed tokens
 */
char **csh_split_line(char *line);

#endif // CSH_LINEH
