#include <stddef.h>
#include <stdint.h>
#include <kheap.h>
#include <memory.h>
#include <string.h>

extern void kprintf(const char *str, ...);

uintptr_t block_location;
memory_block_t *blocks;
uint32_t heap_start;
uintptr_t heap_end;

uintptr_t cur;

/**
 * 
 * Allocate memory to the heap
 * 
 * @param  size [description]
 * @return      [description]
 */
uint32_t malloc(unsigned int size) 
{
	uintptr_t ret = cur;
	cur += size;
	return (uint32_t) ret;
}

void free(void *ptr) 
{
}

/**
 *
 * Installs the heap at a given address
 * @param address the starting address
 *
**/
void heap_install(uintptr_t address) {
	cur = address; 
}