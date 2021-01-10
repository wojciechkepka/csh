#include "str.h"

String *empty_str(const size_t capacity)
{
    String *str = malloc(sizeof(String));
    if (!str) return NULL;
    str->buf = malloc(capacity * sizeof(char));
    if (!str->buf) return NULL;
    str->capacity = capacity;
    str->len = 0;
    str->buf[0] = '\0';

    return str;
}

void free_str(String *str)
{
    free(str->buf);
    free(str);
}

void append_str(String *str, const char *format, ...)
{
    va_list args;
    char *s;

    va_start (args, format);
    vasprintf(&s, format, args);
    va_end (args);

    if (str->len + strlen(s) >= str->capacity)
    {
        str->capacity += str->capacity;
        str->buf = realloc(str->buf, str->capacity);
    }

    str->len += (size_t)sprintf(str->buf + str->len, "%s", s);
    str->buf[str->len] = '\0';
    free(s);
}
