#include <iostream>
#include <getopt.h>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**, char**);


    char * pathName = nullptr;
    char * secondaryPathName = nullptr;

    //if no arguments provided, print error message, do not forc, do not pipe, do not exec, end process here
    if (!HandleOptions(argc, argv, &pathName, &secondaryPathName)) {
		cout << "Missing required command line option -1" << endl;
		return 1;
	}

    

    return 0;

}

bool HandleOptions(int argc, char ** argv, char** pName, char** secondPName) {
    int c;

    //checks args to see if they mach any valid command line options, execute switch statements if this is the case
    while ((c = getopt(argc, argv, "1:dt:vi:o:a:2:")) != -1) {
		switch (c) {
            case '1': //if case '-1' is present
            *pName = optarg;

            int rc = fork(); //initiate fork

            if (rc < 0) // if the fork failed, print error and exit
            { 
                fprintf(stderr, "fork failed\n");
            }
            else if (rc == 0) //if fork was successful
            {
                printf("Child 1: pid:%d\n", (int) getpid()); 
                /* figure out way to increment the number next to "Child" based on whether we're piping from one program to the other or not, 
                like the sample output Perry gave us in the project specs */

                execvp(optarg); //first draft of exec call, also checks if the path leads to an executable file

                cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
            }

            break;

            case '2': //if case '-2' is present
            *secondPName = optarg;
            break;


        }


    }

}