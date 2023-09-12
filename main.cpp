#include <iostream>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**, char**);
    void printError();


    char * pathName = nullptr;
    char * fileName = nullptr;

    //if no arguments provided, print error message, do not fork, do not pipe, do not exec, end process here
    if (!HandleOptions(argc, argv, &pathName, &fileName)) {
		printError(); //this keeps executing even when the right arguments are given and i'm not sure why
		return 1;
	}
    

    return 0;

}

void printError()
{
    cout << "Missing required command line option -1" << endl;


}

bool HandleOptions(int argc, char ** argv, char** pName, char** fName) {
    int c;

    //int p[2];
    //for implementing pipes

    char *myargv[2]; //array of char pointers

    //checks args to see if they mach any valid command line options, execute switch statements if this is the case
    while ((c = getopt(argc, argv, "1:dt:vi:o:a:2:")) != -1) {
		switch (c) {
            {
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

                    myargv[0] = *pName; //program to execute, either ls or wc depending on cmd line args
                    myargv[1] = *fName; //file that ls or wc would be reading from, main.cpp is placeholder. -o and -a would replace this value with whatever file we want to read

                    execvp(*pName, fName); //first draft of exec call, also checks if the path leads to an executable file

                    cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
                }

            }

            break;

            /* case '2': //if case '-2' is present
            *secondPName = optarg;
            break; */




        }


    }

    return 0;

}