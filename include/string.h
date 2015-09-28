#ifndef __STRING_H
#define __STRING_H

#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, int val, size_t count);
void *memsetw(void *dest, uint16_t val, size_t count);

size_t strlen(const char *text);
char *strcpy(char *dest, const char *src);

#endif