#include <iostream>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**, char**);
    void printError();


    char * pathName = 0;
    char * fileName = 0;

    //if no arguments provided, print error message, do not fork, do not pipe, do not exec, end process here
    if (HandleOptions(argc, argv, &pathName, &fileName) == false) {
		printError();
		return 1;
	}

    char * myargv[2]; //array of char pointers

    int rc = fork(); //initiate fork

    if (rc < 0) // if the fork failed, print error and exit
    { 
        fprintf(stderr, "fork failed\n");

        exit(1); 
    }
    else if (rc == 0) //if fork was successful
    {
        printf("Child 1: pid:%d\n", (int) getpid()); 

        myargv[0] = strdup(pathName); //program to execute, either ls or wc depending on cmd line args
        myargv[1] = 0; //file that ls or wc would be reading from

        /* if (fileName == 0)
        {
            myargv[1] = 0;
        }
        else
        {
            myargv[1] = strdup(fileName);
        } */
        /*
          if you use -i, this if statement causes the exec call to fail. any ideas on why this is? */
        
        execvp(myargv[0], myargv); //first draft of exec call, also checks if the path leads to an executable file

        cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
    }
    else 
    {
        wait(NULL);
    }



    return 0;

}

void printError()
{
    cout << "Missing required command line option -1" << endl;


}

bool HandleOptions(int argc, char ** argv, char** pName, char** fName) {
    int c;

    bool argTrue = false;

    //int p[2];
    //for implementing pipes

    //checks args to see if they mach any valid command line options, execute switch statements if this is the case
    while ((c = getopt(argc, argv, "1:Dt:vi:o:a:2:")) != -1) {
        argTrue = true;
        

		switch (c){
            default:
            case '1': //if '-1' is present
            {
                *pName = optarg;
    
                break;

            }

            case 'i': //if '-i' is present
            {
                *fName = optarg;
                break;

            }
            case 'D': //if '-D' is present
            {
                char cwd[128];
                cout << getcwd(cwd, sizeof(cwd)) << endl;

            }

        }


    }

    return argTrue;

}