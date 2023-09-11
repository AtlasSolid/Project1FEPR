#include <iostream>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**);


    char * pathName = nullptr;

    //if no arguments provided, print error message, do not fork, do not pipe, do not exec, end process here
    if (!HandleOptions(argc, argv, &pathName)) {
		cout << "Missing required command line option -1" << endl; //this keeps executing even when the right arguments are given and i'm not sure why
		return 1;
	}
    

    return 0;

}

bool HandleOptions(int argc, char ** argv, char** pName) {
    int c;

    //int p[2];
    //for implementing pipes

    //checks args to see if they mach any valid command line options, execute switch statements if this is the case
    while ((c = getopt(argc, argv, "1:dt:vi:o:a:2:")) != -1) {
		switch (c) {
            case '1': //if case '-1' is present
            *pName = optarg;

            int rc = fork(); //initiate fork

            if (rc < 0) // if the fork failed, print error and exit
            { 
                fprintf(stderr, "fork failed\n");

                exit(1); 
            }
            else if (rc == 0) //if fork was successful
            {
                printf("Child 1: pid:%d\n", (int) getpid()); 
                /* figure out way to increment the number next to "Child" based on whether we're piping from one program to the other or not, 
                like the sample output Perry gave us in the project specs */

                char *myargs[3]; //array of char pointers

                myargs[0] = strdup(optarg); //program to execute, either ls or wc depending on cmd line args
                myargs[1] = strdup("main.cpp"); //file that ls or wc would be reading from, main.cpp is placeholder. -o and -a would replace this value with whatever file we want to read
                myargs[2] = NULL;

                execvp(myargs[0], myargs); //first draft of exec call, also checks if the path leads to an executable file

                cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
            }

            break;

            /* case '2': //if case '-2' is present
            *secondPName = optarg;
            break; */


        }


    }

    return 0;

}