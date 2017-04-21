#include <stdio.h>
#include <stdlib.h>

int main()
{
    char command[80] = "date +%A";
    char name[32];

    printf("Enter your nickname: ");
    gets(name);
    printf("Hi, %s.  Today is: ", name);
	printf("Command : %s\n", command);
    fflush(stdout);
    system(command);
    return 0;
}
