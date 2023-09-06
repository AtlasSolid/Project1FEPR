#include <iostream>
#include <getopt.h>
#include <string>

using namespace std;

int main(int argc, char * argv[]) {

    //VERY barebones fork, definitely will be reworked.
    //at some point we'll make sure it only executes based on the command line args (-1, -2)

    int rc = fork(); //initiate fork

    if (rc < 0) // if the fork failed, print error and exit
    { 
        fprintf(stderr, "fork failed\n");
    }
    else if (rc == 0) //if fork was successful
    {
        printf("Child 1: pid:%d\n", (int) getpid()); 
        /*figure out way to increment the number next to "Child" based on whether we're piping from one program to the other or not, 
        like the sample output Perry gave us in the project specs*/
    }

}