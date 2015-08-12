#include <stdint.h>
#include <stdlib.h>
#include <multiboot.h>
#include <kheap.h>
#include <stdio.h>

void _Exit(int status)
{
	printf("_exit(%d)\n", status);
	for(;;);
}

int main(int argc, char *argv[]);

void kmain(multiboot_t *multiboot, uint32_t magic) {

	extern void *end;
    heap_install((uintptr_t) &end);

    main(0, NULL);

    for(;;);
}