#include <limits.h>
#include "printf.h"

#define PRINT_BUF_MAX	40

int putchar(char);

void printc(char *buf, size_t lim, size_t *pos, char c) {
	if (*pos < lim - 1)
	    buf[(*pos)++] = c;
	buf[*pos] = '\0';
}

void prints(char *buf, size_t lim, size_t *pos, char *str) {
	while (*str != '\0') {
		if (*pos == lim - 1)
			break;
		buf[(*pos)++] = *str++;
	}
	buf[*pos] = '\0';
}

void printu(char *buf, size_t lim, size_t *pos, size_t base, unsigned long num) {
	static char *base_chars = "0123456789ABCDEF";
	unsigned long fac = num / base;

	if (fac != 0)
		printu(buf, lim, pos, base, fac);
	printc(buf, lim, pos, base_chars[num%base]);
}

void printn(char *buf, size_t lim, size_t *pos, size_t base, long num) {
	if (num < 0) {
		num = -num;
		printc(buf, lim, pos, '-');
	}
	printu(buf, lim, pos, base, (unsigned long)num);
}

size_t vsnprintf(char *buf, size_t lim, const char *fmt, va_list args) {
	size_t pos = 0;

	for (;;) {
		char c;
		while ((c = *fmt++) != '%') {
			if (c == '\0')
				goto out;
			printc(buf, lim, &pos, c);
		}

		c = *fmt++;
		switch (c) {
		case 'c':
			printc(buf, lim, &pos, (char)va_arg(args, int));
			break;
		case 's':
			prints(buf, lim, &pos, (char *)va_arg(args, const char *));
			break;
		case 'i':
		case 'd':
			printn(buf, lim, &pos, 10, (long)va_arg(args, int));
			break;
		case 'p':
			prints(buf, lim, &pos, "0x");
			/* fall through */
		case 'x':
			printn(buf, lim, &pos, 16, (long)va_arg(args, int));
			break;
		case 'u':
			printu(buf, lim, &pos, 10,
			       (unsigned long)va_arg(args, unsigned int));
			break;
		case 'o':
			printc(buf, lim, &pos, '0');
			printu(buf, lim, &pos, 8,
			       (unsigned long)va_arg(args, unsigned int));
			break;
		case '%':
			printc(buf, lim, &pos, '%');
			break;
		default:
			printc(buf, lim, &pos, '%');
			printc(buf, lim, &pos, c);
			break;
		}
	}

out:
   	return pos;
}

int snprintf(char *buf, size_t lim, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, lim, fmt, args);
	va_end(args);
	return 0;
}

int printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	char buf[PRINT_BUF_MAX] = {0};
	size_t i = 0;
	
	vsnprintf(buf, sizeof(buf), fmt, args);
	while (buf[i])
		putchar(buf[i++]);

	va_end(args);
	return 0;
}

int sprintf(char *buf, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	vsnprintf(buf, LONG_MAX, fmt, args);
	va_end(args);
	return 0;
}
