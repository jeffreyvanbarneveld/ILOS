#ifndef __KHEAP_H
#define __KHEAP_H

#include <stdint.h>

#define HEAP_BLOCKS 0x100

/* Structure of one memory block */
typedef struct memory_block
{
    uint8_t   used;
    size_t    size;
    uintptr_t address;
} memory_block_t;

void heap_install();
void *malloc(size_t size);
void free(void* ptr);

#endif