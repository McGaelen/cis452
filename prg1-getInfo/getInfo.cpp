#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#define MAX_NAME_LENGTH 15   // max length for a hostname
#define NUM_PASSWD_ENTRIES 5 // how many passwd entries we want to get


int main() {

    //store username
    std::string username = getlogin();
    //store uid
    int uid = getuid();
    //store gid
    int gid = getgid();
    //store hostname (using temp because gethostname() wants a char array only)
    char temp[MAX_NAME_LENGTH];
    gethostname(temp, MAX_NAME_LENGTH);
    std::string hostname = temp;
    hostname = hostname.substr(0, hostname.find("."));
    //print all stored values
    std::cout << std::setw(10) << "User is: " << username << std::endl;
    std::cout << std::setw(10) << "UID is: " << uid << std::endl;
    std::cout << std::setw(10) << "GID is: " << gid << std::endl;
    std::cout << std::setw(10) << "Host is: " << hostname << std::endl;

    //Get first NUM_PASSWD_ENTRIES from passwd
    struct passwd *p;
    for (int i = 0; i < NUM_PASSWD_ENTRIES; i++) {
    	p = getpwent();       // getpwent() iterates every time it's called
    	std::cout << std::setw(7) << "entry " << i << ": "<< p->pw_name << std::endl;
    }

    //now ask for environment variables
    std::string env;
    std::cout << "Environment variable desired? ";
    std::cin >> env;
    char *value = getenv(env.c_str()); //use char array so we can compare to null
    //get trapped in this loop if environment var entered doesn't exist
    while (value == NULL) {
        std::cout << "Please enter a valid environment variable: ";
        std::cin >> env;
        value = getenv(env.c_str());
    }
    std::cout << "value is: " << value;

    return 0;
}
