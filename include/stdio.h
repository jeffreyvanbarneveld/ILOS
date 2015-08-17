#ifndef __STDIO_H
#define __STDIO_H

#include <stdint.h>

void printf(const char* str, ...);
void perror(char* str);
void screen_scroll();
void putch(char c);
void screen_writeline(char *text);
void screen_write(char *text);
void screen_write_double(double val);
void printf(const char *str, ...);
void screen_write_hex(uint32_t val);
void screen_write_dec(int32_t val);

#endif