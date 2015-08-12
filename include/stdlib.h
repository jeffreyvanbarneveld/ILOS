#ifndef __STDLIB_H
#define __STDLIB_H

#define NULL ((void*)0)
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include <kheap.h>
#include <memory.h>

void _Exit(int status);

#endif