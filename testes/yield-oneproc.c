/* This tests the behaviour of yield with a single process of high priority.
 *
 * The program receives a number from standard input, or uses 10
 * as the default, and is supposed to print all numbers from 1 to
 * the given one.
 *
 * There are two threads with different priorities. The high-priority
 * thread prints the numbers, yielding after every one. The low-priority
 * thread ends the execution.
 *
 * If the test fails, exits with 1;
 */
#include <stdlib.h>
#include <stdio.h>
#include "unucleo.h"

#define DEFAULT_MAX 10
int max;
int obtained;

void *count(void *first) {
	obtained = (int)first;
	while (obtained <= max) {
		printf("%d ", obtained++);
		mproc_yield();
	}

	return NULL;
}

void *end(void *expected) {
	puts("");

	if ((int)expected == obtained) {
		puts("Everything went as expected!");
		exit(0);
	} else {
		printf("Error: expected to end on %d, but got %d\n",
			   (int)expected, obtained);
		exit(1);
	}

	return NULL;
}

int main(int argc, char **argv)
{
	/* Obtains the maximum number from command line options */
	if (argc < 2 || sscanf(argv[1], "%d", &max) < 1) {
		max = DEFAULT_MAX;
	}

	/* Verifies if the given number is valid */
	if (max < 1) {
		printf("Don't troll me è.é\n"
			   "Give me a number greater than 1 or\n"
			   "i'll go into an infinite loop.\n"
			   "\n"
			   "I'm using %d instead of %d.\n",
			   DEFAULT_MAX, max);
		max = DEFAULT_MAX;
	}

	/* Runs the threads */
	libsisop_init();
	mproc_create(1, count, 1);
	mproc_create(2, end, max+1);

	scheduler();

	/* Not supposed to reach this */
	puts("Error: a thread should have already called exit.");
	return 1;
}
