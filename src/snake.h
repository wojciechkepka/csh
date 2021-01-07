#ifndef CSH_SNAKEH
#define CSH_SNAKEH

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include "csh.h"
#include "ansi.h"
#include "line.h"


int play_snake(Csh *csh, char **args);

#endif // CSH_SNAKEH
