/* This tests the alternation of two processes with the same priority.
 *
 * The program receives a number from standard input, or uses 10
 * as the default, and is supposed to print all numbers from 1 to
 * the given one.  One thread prints the odd numbers, another the
 * even, and the order must be correct.
 *
 * If the test fails, exits with 1;
 */
#include <stdio.h>
#include <stlib.h>
#include "unucleo.h"

#define DEFAULT_MAX = 10;
int max;

int current;
int *expected;
int *obtained;

void printnums(void *first)
{
	int i = (int)first;
	while (i<=max) {
		printf("%d ", i++);
		obtained[current++] = i;
		mproc_yield();
	}
}

int main(int argc, char **argv)
{
	/* Obtains the maximum number for command line options */
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

	/* Allocates the arrays for testing */
	expected = calloc(max, sizeof(int));
	obtained = calloc(max, sizeof(int));

	/* Initializes the expected numbers */
	for (int i=1; i <= max; i++) {
		expected[i-1] = i;
	}

	/* Runs the threads */
	libsisop_init();
	mproc_create(1, printnums, 1);
	mproc_create(1, printnums, 2);

	scheduler();
	puts("");

	/* Verifies the results */
	bool ok=true;
	for (int i=0; i < max; i++) {
		if (obtained[i] != expected[i]) {
			printf("Error: expected %d but got %d.\n", expected[i], obtained[i]);
			ok=false;
		}
	}

	if (ok) {
		puts("Everything went as expected :)");
		return 0;
	} else {
		return 1;
	}
}
