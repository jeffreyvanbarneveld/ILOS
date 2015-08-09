#include <stddef.h>
#include <stdint.h>
#include <memory.h>


/**
 *
 * Copies memory from src to dest
 * @param dest the destination location
 * @param src the source location
 * @param count the amount to copy
 *
**/
void memcpy(void *dest, void *src, size_t count) {
    // Something to copy?
    if(count == 0)
        return;

    // Copy sizeof(uint64_t) bytes per loop
    while(count >= sizeof(uint64_t)) {
        *(uint64_t *)dest = *(uint64_t *)src;

        dest  += sizeof(uint64_t);
        src   += sizeof(uint64_t);
        count -= sizeof(uint64_t);
    }

    // Copy sizeof(uint32_t) bytes per loop
    while(count >= sizeof(uint32_t)) {
        *(uint32_t *)dest = *(uint32_t *)src;

        dest  += sizeof(uint32_t);
        src   += sizeof(uint32_t);
        count -= sizeof(uint32_t);
    }

    // Copy sizeof(uint16_t) bytes per loop
    while(count >= sizeof(uint16_t)) {
        *(uint16_t *)dest = *(uint16_t *)src;

        dest  += sizeof(uint16_t);
        src   += sizeof(uint16_t);
        count -= sizeof(uint16_t);
    }

    // Copy sizeof(uint8_t) byte per loop
    while(count >= sizeof(uint8_t)) {
        *(uint8_t *)dest = *(uint8_t *)src;

        dest  += sizeof(uint8_t);
        src   += sizeof(uint8_t);
        count -= sizeof(uint8_t);
    }
}


/*
*
*	Compare 2 char pointers n times
*
*/
int memcmp(const char *s1, const char *s2, size_t n)
{

	unsigned char u1, u2;

	for (; n--; s1++, s2++) 
	{
		u1 = *(unsigned char *)s1;
		u2 = *(unsigned char *)s2;
		
		if (u1 != u2)
			return (u1 - u2);
	}
	
	return 0;
}

/**
 *
 * Sets a part of the memory to the given value
 * @param dest the destination location in memory
 * @param val the value to set it to
 * @param count the amount of bytes to set
 *
**/
void memset(void *dest, const uint8_t val, size_t count) {
    __asm__ __volatile__("cld; rep stosb" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
}

/**
 *
 * Sets a part of the memory to the given value
 * @param dest the destination location in memory
 * @param val the value to set it to
 * @param count the amount of words to set
 *
**/
void memsetw(void *dest, const uint16_t val, size_t count) {
    __asm__ __volatile__("cld; rep stosw" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
}

/**
 *
 * Sets a part of the memory to the given value
 * @param dest the destination location in memory
 * @param val the value to set it to
 * @param count the dwords of bytes to set
 *
**/
void memsetd(void *dest, const uint32_t val, size_t count) {
    __asm__ __volatile__("cld; rep stosl" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
}