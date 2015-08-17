#include <stdint.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>

uint16_t *textmemptr = (uint16_t *) 0xB8000;
uint16_t attrib = 0x07;
int8_t   csr_x = 0;
int8_t   csr_y = 0;

/**
 *
 * Print an error to the screen
 * @param str the error string
 *
**/
void perror(char *str)
{
    printf("PERROR(%s)\n", str);
}

/**
 *
 * Scrolls the screen
 *
**/
void screen_scroll()
{
    unsigned short blank = ' ' | (attrib << 8);
    if(csr_y >= 25)
    {
        uint8_t temp = csr_y - 24;
        
        memcpy(textmemptr, (void *) (textmemptr + temp * 80), (25 - temp) * 80 * 2);
        memsetw(textmemptr + (25 - temp) * 80, blank, 80);
        
        csr_y = 24;
    }
}

/**
 *
 * Puts a character on the screen
 * @param c the character
 *
**/
void putch(char c)
{
    unsigned att = attrib << 8;

    /* Backspace */
    if(c == '\b')
    {
        if(csr_y > 0)
        {
            uint16_t* where = textmemptr + (csr_y * 80 + csr_x - 1);
            *where = ' ' | att;
            csr_x--;

            /* If we need to go back to previous line */
            if(csr_x < 0)
            {
                csr_x = 80 - 1;
                csr_y--;
            }
        }
    }
    /* Tab */
    else if(c == '\t')
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Linefeed */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* Newline */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Print character */
    else
    {
        uint16_t* where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;   
        csr_x++;
    }

    /* New line? */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }
    
    /* Scroll screen if needed */
    screen_scroll();
}

/**
 *
 * Writes text to the screen
 * @param text the text
 *
**/
void screen_write(char* text)
{
    /* Put each character on screen until a \0 is found */
    while(*text)
        putch(*text++);
}

/**
 *
 * Prints a double to the screen
 * @param val the double
 *
**/
void screen_write_double(double val)
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
    screen_write_dec(wholes);
    screen_write(".");

    /* Calculate double part of number */
    double leftover = val - wholes;

    /* Check if there's a number after the comma */
    leftover *= 10;
    if((int) leftover == 0)
    {
        putch('0');
        return;
    }

    /* Write number after comma */
    while((int) leftover != 0)
    {
        int num = (int) leftover;
        leftover -= num;
        screen_write_dec(num);
        leftover *= 10;
    }
}

/**
 *
 * Print formatted string to screen
 * @param str the formatted string
 * @param ... the data
 *
**/
void printf(const char *str, ...)
{
    va_list args;
    va_start(args, str);
    
    /* Loop through each character */
    for(uint32_t i = 0; str[i] != '\0'; i++)
    {
        /* No special case: print to screen */
        if(str[i] != '%')
        {
            putch(str[i]);
        }
        else
        {
            /* Skip % */
            i++;

            /* Print to screen */
            switch(str[i])
            {
                case 's':
                    screen_write((char*) va_arg(args, char*));
                    break;
                    
                case 'g':
                    screen_write_double((double) va_arg(args, double));
                    break;

                case 'x':
                    screen_write_hex((uint32_t) va_arg(args, uint32_t));
                    break;
                    
                case 'd':
                    screen_write_dec((int32_t) va_arg(args, int32_t));
                    break;
                
                case 'c':
                    putch((char) va_arg(args, int));
                    break;
                
                case '%':
                    putch('%');
                    break;
                
                default:
                    putch(str[i]);
                    break;
            }
        }
    }
    
    va_end(args);
}

/**
 *
 * Prints a hexadecimal value to the screen
 * @param val the number
 *
**/
void screen_write_hex(uint32_t val)
{
    /* Special case for zero */
    if(val == 0)
    {
        putch('0');
        return;
    }

    char noZeroes = 1;
    for(int32_t i = 28; i >= 0; i -= 4)
    {
        /* Get this part of the number */
        uint16_t tmp = (val >> i) & 0x0F;

        /* If we don't have a non-zero part of the number yet */
        if(tmp == 0 && noZeroes != 0)
            continue;
        
        /* Print */
        noZeroes = 0;
        if(tmp >= 0x0A)
            putch(tmp - 0x0A + 'A');
        else
            putch(tmp + '0');
    }
}

/**
 *
 * Writes an integer value to the screen
 * @param val the value
 *
**/
void screen_write_dec(int32_t val)
{
    /* Special case for zero */
    if(val == 0)
    {
        putch('0');
        return;
    }

    /* Negative value? */
    if(val < 0)
    {
        val = -val;
        putch('-');
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
        screen_write_dec(v);
    
    putch(n + '0');
}