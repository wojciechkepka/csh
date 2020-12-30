#ifndef CSH_TERMH
#define CSH_TERMH

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

#endif //CSH_TERMH