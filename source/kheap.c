#include <stddef.h>
#include <stdint.h>
#include <kheap.h>
#include <memory.h>
#include <string.h>

extern void kprintf(const char *str, ...);

uintptr_t block_location;
memory_block_t *blocks;
uint32_t heap_start;

/**
 * 
 * Allocate memory to the heap
 * 
 * @param  size [description]
 * @return      [description]
 */
void *malloc(unsigned int size) 
{
	if (size <= 0)
		return 0;

	uint32_t index = 0;
	uint32_t offset = 0;
	while (index < HEAP_BLOCKS) 
	{
		// Get block
		memory_block_t *block = &blocks[index];

		/**
		*
		* Block used?
		*
		**/
		if (block->used) 
		{
			offset += block->size;
			index++;
			continue;
		}

		if (block->size >= size || block->size == 0) 
		{
			block->used = 1;
			block->address = offset;
			if (block->size == 0)
				block->size = size;


			return (void *)(heap_start + offset);
		}

		offset += block->size;
		index++;
	}

	kprintf("HEAP OUT OF MEMORY, PANIC PANIC!!!");
	for (;;);
	return 0;
}

void free(void *ptr) 
{

	// Invalid pointer?
	if (ptr == NULL)
		return;

	//kprintf("%x\n", (uint32_t)ptr);

	// Calculate the address
	uint32_t address = (uint32_t)ptr - heap_start;

	// Loop through blocks and find offset from address
	uint32_t index = 0;
	while (index < HEAP_BLOCKS) 
	{
		// Get block
		memory_block_t *block = &blocks[index];

		// Address correct? Mark as unused!
		if (block->used && block->address == address) 
		{
			block->used = 0;
			return;
		}

		// Next up
		index++;
	}
}

/**
 *
 * Installs the heap at a given address
 * @param address the starting address
 *
**/
void heap_install(uintptr_t address) {
	// Set heap
	uint32_t start = address;
	heap_start = start + 0x1000;
	block_location = heap_start;

	// Allocate blocks
	uint32_t blocksize = sizeof(memory_block_t) * HEAP_BLOCKS;
	heap_start += blocksize;

	// Initialise memory blocks
	memset((void *)block_location, 0, blocksize);
	blocks = (memory_block_t *)block_location;
}