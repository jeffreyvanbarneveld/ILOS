#include <stdint.h>
#include <stdlib.h>
#include <multiboot.h>
#include <kheap.h>
#include <stdio.h>
#include <runtime.h>

/**
 *
 * Exit the VM with a status code
 * @param status the status code
 *
**/
void _Exit(int status)
{
    printf("Exit with status %d\n", status);
    while(1)
        asm volatile("hlt");
}

/**
 *
 * Kernel entrypoint
 * @param multiboot pointer to multiboot header
 * @param magic     magic value from multiboot bootloader
 *
**/
void kmain(multiboot_t* multiboot, uint32_t magic)
{
    /* Check magic */
    if(magic != 0x2BADB002)
    {
        printf("Multiboot magic is incorrect.\n");
        _Exit(EXIT_FAILURE);
    }

    /* Check if there are modules */
    if(multiboot->mods_count == 0)
    {
        printf("No modules available. Can't boot.\n");
        _Exit(EXIT_FAILURE);
    }

    /* Get module and info about it */
    multiboot_module_t* module = (multiboot_module_t*) multiboot->mods_addr;
    uintptr_t start = module->mod_start;
    uintptr_t end = module->mod_end;

    /* Install heap after module */
    heap_install(end);

    /* Run VM */
    run_vm((void*) start);
}