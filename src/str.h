#ifndef CSH_STRH
#define CSH_STRH

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct String
{
    char *buf;
    size_t capacity;
    size_t len;
} String;

String *empty_str(const size_t capacity);

void free_str(String *str);

void append_str(String *str, const char *format, ...);

#endif // CSH_STRH
