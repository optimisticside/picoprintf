#include <limits.h>
#include "printf.h"

#define PRINT_BUF_MAX	40

int putchar(int);

void printc(char *buf, size_t lim, size_t *pos, int c) {
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

void printu(char *buf, size_t lim, size_t *pos, char *chars,
	    size_t base, unsigned long num) {
	unsigned long fac = num / base;

	if (fac != 0)
		printu(buf, lim, pos, chars, base, fac);
	printc(buf, lim, pos, chars[num%base]);
}

void printn(char *buf, size_t lim, size_t *pos, char *chars,
	    size_t base, long num) {
	if (num < 0) {
		num = -num;
		printc(buf, lim, pos, '-');
	}
	printu(buf, lim, pos, chars, base, (unsigned long)num);
}

size_t vsnprintf(char *buf, size_t lim, const char *fmt, va_list args) {
	static char *lower_chars = "0123456789abcdef";
	static char *upper_chars = "0123456789ABCDEF";
	size_t pos = 0;

	for (;;) {
		int wide = 0;
		char *digits = lower_chars;
		int n, c;
		unsigned int u;

		while ((c = *fmt++) != '%') {
			if (c == '\0')
				goto out;
			printc(buf, lim, &pos, c);
		}

		if ((c = *fmt++) == 'l') {
			wide = 1;
			c = *fmt++;
		}
		if (c == 'X' || c == 'P')
			digits = upper_chars;

		switch (c) {
		case 'c':
			printc(buf, lim, &pos, (char)va_arg(args, int));
			break;
		case 's':
			prints(buf, lim, &pos, (char *)va_arg(args, const char *));
			break;
		case 'i':
		case 'd':
			n = wide ? va_arg(args, long) : (long)va_arg(args, int);
			printn(buf, lim, &pos, digits, 10, n);
			break;
		case 'P':
		case 'p':
			prints(buf, lim, &pos, "0x");
			/* fall through */
		case 'X':
		case 'x':
			n = wide ? va_arg(args, long) : (long)va_arg(args, int);
			printn(buf, lim, &pos, digits, 16, n);
			break;
		case 'u':
			u = (unsigned long)(wide
			                   ? va_arg(args, long) : va_arg(args, int));
			printu(buf, lim, &pos, digits, 10, u);
			break;
		case 'o':
			u = (unsigned long)(wide
			                   ? va_arg(args, long) : va_arg(args, int));
			printc(buf, lim, &pos, '0');
			printu(buf, lim, &pos, digits, 8, n);
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
