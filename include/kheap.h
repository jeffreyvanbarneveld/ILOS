#ifndef __KHEAP_H
#define __KHEAP_H

#define HEAP_BLOCKS         0x100
#define HEAP_INITIAL_SIZE   16

// Structure of one memory block
typedef struct memory_block {
	uint8_t  used;
	uint32_t address;
	uint32_t size;
} memory_block_t;

void heap_install();
uint32_t malloc(unsigned int size);
void free(void *ptr);

#endif
