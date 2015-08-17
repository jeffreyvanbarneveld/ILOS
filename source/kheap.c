#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <kheap.h>
#include <memory.h>
#include <string.h>

/* Info */
uintptr_t block_location;
memory_block_t *blocks;
uintptr_t heap_start;

/**
 * 
 * Allocate memory from the heap
 * 
 * @param  size
 * @return pointer to allocated memory
 *
**/
void *malloc(size_t size) 
{
    /* Nothing to allocate? */
    if(size <= 0)
        return NULL;

    /* Loop through every block */
    uint32_t index = 0;
    uint32_t offset = 0;
    while(index < HEAP_BLOCKS) 
    {
        /* Get block */
        memory_block_t *block = &blocks[index];

        /* Block used? Proceed to next one */
        if(block->used) 
        {
            offset += block->size;
            index++;
            continue;
        }

        /* Block size enough or block never used before? */
        if(block->size >= size || block->size == 0) 
        {
            block->used = 1;
            block->address = offset;
            if(block->size == 0)
                block->size = size;

            return (void *)(heap_start + offset);
        }

        /* Next block */
        offset += block->size;
        index++;
    }

    printf("HEAP OUT OF MEMORY, PANIC PANIC!!!");
    for(;;);
    return 0;
}

/**
 *
 * Frees memory allocated using malloc
 * @param ptr pointer to allocated memory
 *
**/
void free(void* ptr) 
{
    /* Invalid pointer? */
    if (ptr == NULL)
        return;

    /* Calculate the address */
    uintptr_t address = (uintptr_t) ptr - heap_start;

    /* Loop through blocks and find offset from address */
    uint32_t index = 0;
    while (index < HEAP_BLOCKS) 
    {
        /* Get block */
        memory_block_t *block = &blocks[index];

        /* Address correct? Mark as unused! */
        if(block->used && block->address == address) 
        {
            block->used = 0;
            return;
        }

        /* Next up */
        index++;
    }
}

/**
 *
 * Installs the heap at a given address
 * @param address the starting address
 *
**/
void heap_install(uintptr_t address)
{
    /* Set heap address */
    size_t blocksize = (HEAP_BLOCKS * sizeof(memory_block_t));
    heap_start = address + blocksize;
    blocks = (memory_block_t *) address;

    /* Initialise memory blocks */
    memset(blocks, 0, blocksize);
}