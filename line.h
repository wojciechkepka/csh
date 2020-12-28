#ifndef CSH_LINEH
#define CSH_LINEH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "term.h"
#include "prompt.h"

#define CSH_INP_BUF_SIZE 1024
#define CSH_TOK_BUF_SIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"

#define CTRL_C 0x03
#define DEL 0x07F
#define CTRL_L 0x0C
#define CTRL_D 0x04

extern volatile int *GOT_CTRL_C_p;


/* reads from stdin character by character until EOF or '\n' is encountered. By default
 * allocates a buffer of CSH_INP_BUF_SIZE size and extends it as needed. It is the callers duty to
 * free allocated buffer after use.
 *
 * This function may quit the main process if there is an error with allocation or reallocation of
 * the input buffer.
 *
 * @returns a line of input from stdin
 * */
char *csh_readline();

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