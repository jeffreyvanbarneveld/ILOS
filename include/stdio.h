#ifndef __STDIO_H
#define __STDIO_H

#include <stdint.h>

int printf(const char *str, ...);
int puts(const char *text);
void screen_scroll();
void putch(char c);
void screen_write(const char *text);
void screen_write_double(double val);
void screen_write_hex(uint32_t val);
void screen_write_dec(int32_t val);

#endif