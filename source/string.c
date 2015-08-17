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
 * Prints a double to a buffer
 * @param  str the buffer
 * @param  val the double
 * @return the new buffer offset
 *
**/
char *sprintf_write_dec(char *str, int32_t val)
{
    /* Special case for zero */
    if(val == 0)
    {
        *str++ = '0';
        return str;
    }

    /* Negative value? */
    if(val < 0)
    {
        val = -val;
        *str++ = '-';
    }

    /* Get current number */
    uint32_t v = val / 10;
    uint32_t n = val % 10;
    if(n < 0)
    {
        n--;
        v += 10;
    }

    /* Continue with the leftover part of the number */
    if(val >= 10)
        str = sprintf_write_dec(str, v);
    
    *str++ = (char) (n + '0');
    return str;
}

/**
 *
 * Prints a double to a buffer
 * @param  str the buffer
 * @param  val the double
 * @return the new buffer offset
 *
**/
char *sprintf_write_double(char *str, double val)
{
    /* Find position of comma */
    int wholes = (int) val;
    int pos = 0;
    while(wholes > 0)
    {
        wholes /= 10;
        pos++;
    }

    /* Write whole number and comma */
    wholes = (int) val;
    str = sprintf_write_dec(str, wholes);
    *str++ = '.';

    /* Calculate double part of number */
    double leftover = val - wholes;
    
    /* Check if there's a number after the comma */
    leftover *= 10;
    if((int) leftover == 0)
    {
        *str++ = '0';
        return str;
    }

    /* Write number after comma */
    while((int) leftover != 0)
    {
        int num = (int) leftover;
        leftover -= num;
        str = sprintf_write_dec(str, num);
        leftover *= 10;
    }

    return str;
}

/**
 *
 * Prints a hexadecimal value to a buffer
 * @param  str the buffer
 * @param  val the number
 * @return the new buffer offset
 *
**/
char *sprintf_write_hex(char *str, uint32_t hex)
{
    /* Special case for zero */
    if(hex == 0)
    {
        *str++ = '0';
        return str;
    }

    char noZeroes = 1;
    for(int32_t j = 28; j >= 0; j -= 4)
    {
        /* Get this part of the number */
        uint16_t tmp = (hex >> j) & 0x0F;

        /* If we don't have a non-zero part of the number yet */
        if(tmp == 0 && noZeroes != 0)
            continue;
        
        /* Print */
        noZeroes = 0;
        if(tmp >= 0x0A)
            *str++ = tmp - 0x0A + 'A';
        else
            *str++ = tmp + '0';
    }

    return str;
}

/**
 *
 * Prints a string to a buffer
 * @param  str the buffer
 * @param  s   the string
 * @return the new buffer offset
 *
**/
char *sprintf_write(char *str, char *s)
{
    size_t length = strlen(s);
    for(uint32_t j = 0; j < length; j++)
        *str++ = s[j];

    return str;
}

/**
 *
 * Print formatted string to buffer
 * @param  str    the buffer
 * @param  format the formatted string
 * @param  args   the data
 * @return length of resulting string
 *
**/
int vsprintf(char *str, const char *format, va_list args)
{
    uintptr_t start = (uintptr_t) str;
    
    /* Loop through each character */
    for(uint32_t i = 0; format[i] != '\0'; i++)
    {
        /* No special case: print to buffer */
        if(format[i] != '%')
        {
            *str++ = format[i];
        }
        else
        {
            /* Skip % */
            i++;

            /* Print to buffer */
            switch(format[i])
            {
                case 's':
                    str = sprintf_write(str, (char*) va_arg(args, char*));
                    break;

                case 'x':
                    str = sprintf_write_hex(str, (uint32_t) va_arg(args, uint32_t));
                    break;

                case 'd':
                    str = sprintf_write_dec(str, (int32_t) va_arg(args, int32_t));
                    break;

                case 'g':
                    str = sprintf_write_double(str, (double) va_arg(args, double));
                    break;

                case 'c':
                    *str++ = (char) va_arg(args, int);
                    break;

                case '%':
                    *str++ = '%';
                    break;

                default:
                    *str++ = format[i];
                    break;
            }
        }
    }
    
    /* Find length */
    uintptr_t end = (uintptr_t) str;
    return (int) (end - start);
}

/**
 *
 * Print formatted string to buffer
 * @param  str    the buffer
 * @param  format the formatted string
 * @param  ...    the data
 * @return length of resulting string
 *
**/
int sprintf(char *str, const char *format, ...)
{
    /* Pass to vssprintf */
    va_list args;
    va_start(args, format);
    uint32_t ret = vsprintf(str, format, args);
    va_end(args);
    
    return ret;
}