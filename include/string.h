#ifndef __STRING_H
#define __STRING_H

#include <stdint.h>

typedef __builtin_va_list   va_list;
#define va_start(ap, last)  __builtin_va_start(ap, last)
#define va_end(ap)          __builtin_va_end(ap)
#define va_arg(ap, type)    __builtin_va_arg(ap, type)
#define va_copy(dest, src)  __builtin_va_copy(dest, src)

int vsprintf(char *str, const char *format, va_list args);
int sprintf(char *str, const char *format, ...);
size_t strlen(const char *text);

#endif