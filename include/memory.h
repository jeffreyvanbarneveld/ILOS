#ifndef __MEMORY_H
#define __MEMORY_H

void memcpy(void *dest, void *src, size_t count);
void memset(void *dest, const uint8_t val, size_t count);
void memsetw(void *dest, const uint16_t val, size_t count);
void memsetd(void *dest, const uint32_t val, size_t count);
void memcpy(void *dest, void *src, size_t count);

#endif