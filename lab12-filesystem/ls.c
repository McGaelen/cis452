#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char **argv)
{

    DIR *dirPtr = NULL;
    struct dirent *dir;
    struct stat stats;

	int show_inode = 0, show_long = 0;
	char relative_dir[4096]; /* the path from the cwd to the directory we want to ls */
	char fullpath[4096]; /* fullpath with the relative path added */

    /* If there's no arguments, look in the current directory */
    if ( argc == 1 ) {
        dirPtr = opendir(".");
        strcpy(relative_dir, ".");
    }
    else {
        /* Loop through each argument, setting each option accordingly.
           If one of the args is a path, open and set the relative path */
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-l") == 0)
				show_long = 1;
			else if (strcmp(argv[i], "-i") == 0)
				show_inode = 1;
			else if (dirPtr == NULL) {
				strcpy(relative_dir, argv[i]);
				dirPtr = opendir(relative_dir);
			}
		}
        /* If dirPtr hasn't been set at this point, use the current directory. */
		if (dirPtr == NULL) {
			dirPtr = opendir(".");
            strcpy(relative_dir, ".");
        }
	}

    /* Loop through the directory */
    while ( (dir = readdir(dirPtr)) ) {
        printf("%-15s  ", dir->d_name);

        /* Reset the fullpath to get the next file out of the relative path */
        *fullpath = '\0';
		strcat(fullpath, relative_dir);
        strcat(fullpath, "/");
        strcat(fullpath, dir->d_name);

        if (stat(fullpath, &stats) < 0) {
            perror("stat() error");
            exit(1);
        }

		/* Print optional inode number */
		if (show_inode) {
			printf("%lu", stats.st_ino);

		}
        /* Print optional long form */
        if (show_long) {
            printf("%-7u", stats.st_uid);
            printf("%-10ld", stats.st_size);
            printf("%-10s", ctime(&stats.st_atime));
        }

		printf("\n");

    }
    closedir(dirPtr);
    return 0;

}
