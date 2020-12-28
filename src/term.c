#include "term.h"

const char *CLEAR_SCREEN_ANSI = "\033[1;1H\033[2J";

void csh_disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ORIG_TERM_SETTINGS);
}

void csh_enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &ORIG_TERM_SETTINGS);
    atexit(csh_disable_raw_mode);

    struct termios raw = ORIG_TERM_SETTINGS;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
    raw.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK);
#pragma GCC diagnostic pop

    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void csh_clear(void)
{
    write(STDIN_FILENO, CLEAR_SCREEN_ANSI, 10);
}