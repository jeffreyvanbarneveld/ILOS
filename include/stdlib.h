#ifndef __STDLIB_H
#define __STDLIB_H

#ifndef NULL
#define NULL ((void*) 0)
#endif
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include <kheap.h>

void _Exit(int status);

#endif