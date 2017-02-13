#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_NAME_LENGTH 15

int main() {

    //store username
    char username[MAX_NAME_LENGTH];
    getlogin_r(username, MAX_NAME_LENGTH);
    //store uid
    int uid = getuid();
    //store gid
    int gid = getgid();
    //store hostname
    char hostname[MAX_NAME_LENGTH];
    char *temp;
    gethostname(temp, MAX_NAME_LENGTH);
    strncpy(hostname, temp, 5);

    //print all stored values
    printf("%10s%s\n", "User is: ", username);
    printf("%10s%i\n", "UID is: ", uid);
    printf("%10s%i\n", "GID is: ", gid);
    printf("%10s%s\n", "Host is: ", hostname);

    return 0;
}
