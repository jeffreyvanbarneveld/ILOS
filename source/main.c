#include <stdint.h>
#include <stdlib.h>
#include <multiboot.h>
#include <kheap.h>
#include <stdio.h>
#include <runtime.h>

void _Exit(int status)
{
    printf("_Exit(%d)\n", status);
    for(;;);
}

void kmain(multiboot_t* multiboot, uint32_t magic)
{
    /* Check magic */
    if(magic != 0x1BADB002)
    {
        printf("Multiboot magic is incorrect.");
        _Exit(EXIT_FAILURE);
    }
    
    /* Check if there are modules */
    if(multiboot->mods_count == 0)
    {
        printf("No modules available. Can't boot.");
        _Exit(EXIT_FAILURE);
    }

    /* Get module and info about it */
    multiboot_module_t* module = (multiboot_module_t*) multiboot->mods_addr;
    uintptr_t start = module->mod_start;
    uintptr_t end   = module->mod_end;

    /* Install heap after module */
    heap_install(end);

    /* Run VM */
    run_vm((void*) start);
}