#include <unistd.h>
#include "printf.h"

/*
 * Provide putchar function that
 * is used internally by printf.
 */
int putchar(int c) {
	write(1, &c, 1);
}

int main(int argc, char *argv[]) {
	printf("Hello\n");
	printf("W%crld!\n", 'o');
	printf("Hello %s!\n", "World");
	printf("I am %d years old.\n", -15);
	printf("My args are at %p\n", (void *)argv);
	printf("This is my %luth sentence.\n", (size_t)6);
	
	char buf[30];
	sprintf(buf, "Hello from line %d!\n", __LINE__);
	printf(buf);

	return 0;
}
