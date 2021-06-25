#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>
#include <stddef.h>

int vsnprintf(char *, size_t, const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int printf(const char *, ...);
int sprintf(char *buf, const char *, ...);

#endif /* PRINTF_H */
