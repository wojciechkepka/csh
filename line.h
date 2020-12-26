#ifndef CSH_LINEH
#define CSH_LINEH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "term.h"

#define CSH_INP_BUF_SIZE 1024
#define CSH_TOK_BUF_SIZE 64
#define CSH_TOK_DELIM " \t\r\n\a"

#define CTRL_C 0x03
#define DEL 0x07F
#define CTRL_L 0x0C
#define CTRL_D 0x04

const char *CLEAR_BACK_CHAR_SEQ;
const char *ANSI_COL_RIGHT;
const char *ANSI_COL_LEFT;
const char ARROW_UP[3];
const char ARROW_DOWN[3];
const char ARROW_RIGHT[3];
const char ARROW_LEFT[3];

char *csh_readline();
char **csh_split_line(char *line);

#endif // CSH_LINEH