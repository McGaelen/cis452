#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Usage: ./ln <filepath> <newlink>
int main (int argc, char **argv)
{
	int sym_link = 0;
	char filepath[4096];
	char newlink[4096];

	// Make sure the strings have nothing in them
	*filepath = '\0';
	*newlink = '\0';

	if (argc < 2) {
		fprintf(stderr, "Usage: ./ln <filepath> <newlink>");
		exit(1);
	}

	/* Process arguments. filepath will always contain the first
	filepath specified, newlink will always conatin the second path. */
	for (int i = 1; i < argc; i++)
	{
		if ( strcmp(argv[i], "-s") == 0 ) {
			sym_link = 1;
		}
		else if ( *filepath == '\0' ) {
			strcpy(filepath, argv[i]);
		}
		else if ( *newlink == '\0' ) {
			strcpy(newlink, argv[i]);
		}
	}

	/* Either symlink or hard link depending on option specified by user. */
	if (sym_link)
	{
		if ( symlink(filepath, newlink) < 0 ) {
			perror("Error symlinking");
			exit(1);
		}
		fprintf(stderr, "File %s sym linked to %s.", filepath, newlink);
	}
	else
	{
		if ( link(filepath, newlink) < 0 ) {
			perror("Error hardlinking");
			exit(1);
		}
		fprintf(stderr, "File %s hard linked to %s.", filepath, newlink);
	}
}
