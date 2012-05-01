/* This tests the maximum allocation of processes.
 *
 * It simply tries to allocate all 128 processes,
 * then tries to allocate one more expecting failure.
 *
 * If the test fails, exits with 1;
 */
#include <stdio.h>
#include "unucleo.h"

void foo()
{
	return;
}

int main()
{
	int result=0;
	libsisop_init();

	/* Creates all 128 allowed processes. */
	for (int i=0; i < 128; i++) {
		if (mproc_create(1, foo, NULL) < 0) {
			printf("Error! Couln't create process number %d\n", i);
			result=1;
		}
	}

	/* Tries one more, expecting failure */
	if (mproc_create(1, foo, NULL) < 0) {
		puts("Couldn't create process number 128, as expected.");
	} else {
		puts("Error! Created the process number 128, which shouldn't be allowed.");
		result = 1;
	}

	return result;
}
