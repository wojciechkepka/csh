#ifndef CSH_TERMH
#define CSH_TERMH

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

const char *CLEAR_SCREEN_ANSI;
static struct termios ORIG_TERM_SETTINGS;

void csh_disable_raw_mode();
void csh_enable_raw_mode();
void csh_clear();


#endif //CSH_TERMH