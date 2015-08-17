#ifndef __OUTPUT_H
#define __OUTPUT_H

#include <stdlib.h>
#include <internal.h>

extern int* methods[];
extern size_t methodsSize[];
extern size_t localsCount[];
extern char** strings[];
extern float* floats[];
extern double* doubles[];
extern int* ints[];
extern long* longs[];
extern runtime_func* methodRefs[];

void load();

#endif