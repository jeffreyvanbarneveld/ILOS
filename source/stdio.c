#include <stdint.h>
#include <string.h>
#include <memory.h>

uint16_t *textmemptr = (uint16_t *) 0xB8000;
uint16_t attrib = 0x07;
uint16_t csr_x = 0, csr_y = 0;



void screen_change_color(uint8_t fg, uint8_t bg);
void screen_clear();
void screen_scroll();
uint16_t screen_get_x();
uint16_t screen_get_y();
void screen_move_cursor_to(uint16_t x, uint16_t y) ;
void putch(char c) ;
void screen_writeline(char *text);
void screen_write(char *text);
void screen_writel(char *src, uint32_t len);
void printf(const char *str, ...);
void screen_write_hex(uint32_t val) ;
void screen_write_dec(uint32_t val) ;


void perror(char *str)
{
	printf("PERROR(%s)\n", str);
}


void move_cursor() {
}

void screen_change_color(uint8_t fg, uint8_t bg) {
	attrib = (bg << 4) | (fg & 0x0F);
}

void screen_clear() {
	csr_x = 0;
	csr_y = 0;

	move_cursor();

	uint32_t i = 0;
	unsigned blank = 0x20 | (attrib << 8);
	for(; i < 80 * 25; i++)
		textmemptr[i] = blank;
}

void screen_scroll() {
	unsigned blank, temp;
	
	blank = 0x20 | (attrib << 8);
	
	if(csr_y >= 25) {
		temp = csr_y - 24;
		
		memcpy(textmemptr, (void *) (textmemptr + temp * 80), (25 - temp) * 80 * 2);
		memsetw(textmemptr + (25 - temp) * 80, blank, 80);
		
		csr_y = 24;
	}
}

uint16_t screen_get_x() {
	return csr_x;
}

uint16_t screen_get_y() {
	return csr_y;
}

void screen_move_cursor_to(uint16_t x, uint16_t y) {
	csr_x = x;
	csr_y = y;
	move_cursor();
}

void putch(char c) {
    uint16_t *where;
    unsigned att = attrib << 8;

    if(c == '\b') {
        if(csr_x != 0) {
			where = textmemptr + (csr_y * 80 + csr_x - 1);
			*where = ' ' | att;	
			csr_x--;
		}
    } else if(c == '\t') {
        csr_x = (csr_x + 8) & ~(8 - 1);
    } else if(c == '\r') {
        csr_x = 0;
    } else if(c == '\n') {
        csr_x = 0;
        csr_y++;
    } else if(c >= ' ') {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	
        csr_x++;
    }

    if(csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }
	
	screen_scroll();
	move_cursor();
}

void screen_writeline(char *text) {
	screen_write(text);
	putch('\n');
}

void screen_write(char *text) {
    uint32_t i = 0;
    while(text[i] != '\0')
    	putch(text[i++]);
}

void screen_writel(char *src, uint32_t len) {
	while(len > 0) {
		putch(*src++);
		len--;
	}
}
void screen_write_double(double val) {

	int wholes = (int)val;

	int pos = 0;
	while(wholes > 0)
	{
		wholes /= 10;
		pos++;
	}

	wholes = (int) val;
	screen_write_dec(wholes);
	screen_write(".");

	double leftover = val - wholes;
	

	leftover *= 10;
	if((int) leftover == 0)
	{
		putch('0');
		return;
	}

	while((int)leftover != 0)
	{
		int num = (int)leftover;
		leftover -= num;
		screen_write_dec(num);
		leftover *= 10;
	}
}

void printf(const char *str, ...) {
	va_list args;
	va_start(args, str);
	
	uint32_t i = 0;
	for(; str[i] != '\0'; i++) {
		if(str[i] != '%') {
			putch(str[i]);
		} else {
			i++;
			switch(str[i]) {
				case 's':
					screen_write((char *) va_arg(args, char *));
					break;
					
				case 'g':
					screen_write_double((double) va_arg(args, double));
					break;

				case 'x':
					screen_write_hex((uint64_t) va_arg(args, uint64_t));
					break;
					
				case 'd':
					screen_write_dec((uint64_t) va_arg(args, uint64_t));
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

void screen_write_hex(uint32_t val) {
	if(val == 0) {
		putch('0');
		return;
	}

	uint16_t tmp;
	char noZeroes = 1;

	int32_t i = 28;
	for(; i >= 0; i -= 4) {
		tmp = (val >> i) & 0x0F;
		if(tmp == 0 && noZeroes != 0)
			continue;
		
		noZeroes = 0;
		if(tmp >= 0x0A)
			putch(tmp - 0x0A + 'A');
		else
			putch(tmp + '0');
	}
}

void screen_write_dec(uint32_t val) {
	if(val == 0) {
		putch('0');
		return;
	}

	uint32_t v = val / 10;
	uint32_t n = val % 10;
	
	if(n < 0) {
		n--;
		v += 10;
	}
	
	if(val >= 10)
		screen_write_dec(v);
	
	putch(n + '0');
}