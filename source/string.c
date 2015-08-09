#include <stdint.h>
#include <string.h>


extern void kprintf(const char *str, ...);


uint32_t strlen(const char *text) {
	uint32_t i = 0;
	
	while(*(text++) != '\0')
		i++;
	
	return i;
}



char *sprintf_write_dec(char *str, uint32_t val) {
	if(val == 0) {
		*str++ = '0';
		return str;
	}

	uint32_t v = val / 10;
	uint32_t n = val % 10;
	
	if(n < 0) {
		n--;
		v += 10;
	}

	if(val >= 10)
		str = sprintf_write_dec(str, v);
	
	*str++ = (char)(n + '0');
	return str;
}

char *sprintf_write_double(char *str, double val) {

	int wholes = (int)val;

	int pos = 0;
	while(wholes > 0)
	{
		wholes /= 10;
		pos++;
	}

	wholes = (int) val;
	str = sprintf_write_dec(str, wholes);
	*str++ = '.';

	double leftover = val - wholes;
	

	leftover *= 10;
	if((int) leftover == 0)
	{
		*str++ = '0';
		return str;
	}

	while((int)leftover != 0)
	{
		int num = (int)leftover;
		leftover -= num;
		str = sprintf_write_dec(str, num);
		leftover *= 10;
	}

	return str;
}


char *sprintf_write_hex(char *str, uint32_t hex) {
	if(hex == 0) {
		*str++ = '0';
		return str;
	}

	uint16_t tmp;
	char noZeroes = 1;

	int32_t j = 28;
	for(; j >= 0; j -= 4) {
		tmp = (hex >> j) & 0x0F;
		if(tmp == 0 && noZeroes != 0)
			continue;
		
		noZeroes = 0;
		if(tmp >= 0x0A)
			*str++ = tmp - 0x0A + 'A';
		else
			*str++ = tmp + '0';
	}

	return str;
}

char *sprintf_write(char *str, char *s) {
	uint32_t length = strlen(s);
	uint32_t j = 0;
	for(; j < length; j++)
		*str++ = s[j];

	return str;
}

uint32_t vssprintf(char *str, const char *format, va_list args) {
	uint32_t i = 0;
	uint32_t start = (uint32_t)str;
	
	
	for(; format[i] != '\0'; i++) {
		if(format[i] != '%') {
			*str++ = format[i];
		} else {
			i++;
			switch(format[i]) {
				case 's':
					str = sprintf_write(str, va_arg(args, char *));
					break;

				case 'x':
					str = sprintf_write_hex(str, (uint64_t)va_arg(args, uint64_t));
					break;

				case 'd':
					str = sprintf_write_dec(str, (uint64_t)va_arg(args, uint64_t));
					break;

				case 'g':
					str = sprintf_write_double(str, (double)va_arg(args, double));
					break;

				case 'c':
					*str++ = (char)va_arg(args, int);
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
	

	uint32_t end = (uint32_t)str;
	return end - start;
}

uint32_t sprintf(char *str, const char *format, ...) {
	va_list args;
	va_start(args, format);
	uint32_t ret = vssprintf(str, format, args);
	va_end(args);
	
	return ret;
}
