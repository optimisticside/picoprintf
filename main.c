#include <unistd.h>
#include "printf.h"

/*
 * Provide putchar function that
 * is used internally by printf.
 */
int putchar(char c) {
	write(1, &c, 1);
}

int main(int argc, char *argv[]) {
	printf("Hello, World!\n");
	printf("Hello from %c.\n", 'C');
	printf("2 + 2 is %d.\n", 2 + 2);
	printf("My args are at %p.\n", (void *)argv);
	
	char buf[30];
	sprintf(buf, "Hello from %s.\n", "sprintf");
	printf(buf);
	return 0;
}
