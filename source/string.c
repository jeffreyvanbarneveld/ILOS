#include <stdint.h>
#include <string.h>

/**
 *
 * Calculates the length of the string
 * @param  text the string
 * @return the length
 *
**/
size_t strlen(const char *text)
{
    size_t i = 0;
    while(text[i])
        i++;
    
    return i;
}

/**
 *
 * String copy
 * @param  dest the destination
 * @param  src  the string to copy from
 * @return the destination
 *
**/
char *strcpy(char *dest, const char *src)
{
    size_t size = strlen(src);
    memcpy(dest, src, size);
    return dest;
}

/**
 *
 * Copies memory from src to dest
 * @param dest  the destination location
 * @param src   the source location
 * @param count the amount to copy
 *
**/
void *memcpy(void *dest, const void *src, size_t count)
{
    /* Something to copy? */
    if(count == 0)
        return dest;

    /* Fast memcpy */
    asm volatile("cld;"
                 "rep movsd;"
                 "mov %2, %3;"
                 "rep movsb"
                 : "+S" (src), "+D" (dest) : "c" (count >> 2), "r" (count & 3) : "memory");

    return dest;
}

/**
 *
 * Compares 2 char pointers n times
 * @param  s1 first pointer
 * @param  s2 second pointer
 * @return difference
 *
**/
int memcmp(const char *s1, const char *s2, size_t n)
{
    for (; (n--) > 0; s1++, s2++) 
    {
        char u1 = *s1;
        char u2 = *s2;
        
        if(u1 != u2)
            return (u1 - u2);
    }
    
    return 0;
}

/**
 *
 * Sets a part of the memory to the given value
 * @param dest  the destination location in memory
 * @param val   the value to set it to
 * @param count the amount of bytes to set
 *
**/
void *memset(void *dest, int val, size_t count)
{
    asm volatile("cld; rep stosb" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
    return dest;
}

/**
 *
 * Sets a part of the memory to the given value
 * @param dest  the destination location in memory
 * @param val   the value to set it to
 * @param count the amount of words to set
 *
**/
void *memsetw(void *dest, uint16_t val, size_t count)
{
    asm volatile("cld; rep stosw" : "+c" (count), "+D" (dest) : "a" (val) : "memory");
    return dest;
}