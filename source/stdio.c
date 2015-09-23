#include <stdint.h>
#include <string.h>
#include <stdio.h>

uint16_t *textmemptr = (uint16_t*) 0xB8000;
uint16_t  attrib = 0x07;
int8_t    csr_x = 0;
int8_t    csr_y = 0;


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
void screen_write(const char *text)
{
    /* Put each character on screen until a \0 is found */
    while(*text)
        putch(*text++);
}

/**
 *
 * Writes text to the screen
 * @param text the text
 *
**/
int puts(const char *text)
{
    screen_write(text);
    putch('\n');
    return 0;
}

/**
 *
 * Print formatted string to screen
 * @param str the formatted string
 * @param ... the data
 *
**/
int printf(const char* str, ...)
{
    static char buffer[1024];

    va_list args;
    va_start(args, str);
    int num = vsprintf(buffer, str, args);
    va_end(args);

    screen_write(buffer);
    return num;
}