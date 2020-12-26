#include "term.h"

const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";

void csh_disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ORIG_TERM_SETTINGS);
}

void csh_enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &ORIG_TERM_SETTINGS);
    atexit(csh_disable_raw_mode);

    struct termios raw = ORIG_TERM_SETTINGS;
    raw.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void csh_clear()
{
    write(STDIN_FILENO, CLEAR_SCREEN_ANSI, 10);
}