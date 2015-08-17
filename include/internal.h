#ifndef __INTERNAL_H
#define __INTERNAL_H

/* Function pointer type */
typedef void(*runtime_func)();

/* Runtime methods */
extern runtime_func runtimeMethods[];

#endif