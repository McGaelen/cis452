#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/* String to hold bytes read from /dev/random */
	char seed[32];
	/* Open /dev/random */
	FILE *randfile = fopen("/dev/random", "r");

	/* Read from /dev/random into seed */
	fgets(seed, 32, randfile);
	/* Cast the bytes read in as a long and use it as the seed */
	srandom((long)seed);

	for (int i = 0; i < 10; i++)
	{
		fprintf(stderr, "%i : %ld\n", i+1, random());
	}
}
