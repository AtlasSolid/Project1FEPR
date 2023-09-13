#include <iostream>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**, char**, char**, char**, char**);
    void printError();


    char * pathName = 0;
    char * secondPathName = 0;
    char * fileName = 0;
    char * oFileName = 0;
    char * aFileName = 0;

    //if no arguments provided, print error message, do not fork, do not pipe, do not exec, end process here
    if (HandleOptions(argc, argv, &pathName, &secondPathName, &fileName, &oFileName, &aFileName) == false) {
		printError();
		return 1;
	}

    int p[2]; //array of two pipe file descriptors

    char * myargv[2]; //array of char pointers

    if (secondPathName == 0)
    {
        int rc = fork(); //parent forks child 1

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
            
            execvp(myargv[0], myargv); //executes ls or wc, also    checks if the path leads to an executable file

            cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
        }
        else 
        {
            wait(NULL); //parent waits for child process to finish, i.e. waits for execvp to properly output to the console
        }

    }
    else
    {
        int pFail = pipe(p);

        if (pFail < 0) //if pipe failed, print error and exit
        {
            fprintf(stderr, "pipe failed\n");

            exit(1); 
        }
        else
        {
            int rc = fork(); //parent forks child 1

            if (rc < 0)
            { 
                fprintf(stderr, "child 1 fork failed\n");

                exit(1); 
            }
            else if (rc == 0)
            {
                close(p[0]); //not reading from the pipe, so close read side
                close(1); //close STDOUT to make it available

                dup(p[1]); //dup write side of the pipe

                close(p[1]); //close original write side of pipe, connection still in File Descriptor 1



                printf("Child 1: pid:%d\n", (int) getpid()); 

                myargv[0] = strdup(pathName);
                myargv[1] = 0;

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
                
                execvp(myargv[0], myargv);

                cout << "Failed exec call" << endl;
            }
            else 
            {
                wait(NULL);
            }

            int rc2 = fork(); //child 1 forks child 2

            if (rc2 < 0)
            { 
                fprintf(stderr, "child 2 fork failed\n");

                exit(1); 
            }
            else if (rc2 == 0)
            {
                close(p[1]); //close write side of the pipe
                close(0); //close STDIN to make it available

                dup(p[0]); //dup read side of the pipe

                close(p[1]); //close original read side of pipe, connection still in File Descriptor 0



                printf("Child 2: pid:%d\n", (int) getpid()); 

                myargv[0] = strdup(pathName);
                myargv[1] = 0;

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
                wait(NULL); //parent waits for child process to finish, i.e. waits for execvp to properly output to the console
            }


        }
        
        close(p[0]);
        close(p[1]);

    }



    return 0;

}

void printError()
{
    cout << "Missing required command line option -1" << endl;


}

bool HandleOptions(int argc, char ** argv, char** pName, char** sPName, char** fName, char** oName, char** aName) {
    int c;

    bool argTrue = false;

    //checks args to see if they mach any valid command line options, execute switch statements if this is the case
    while ((c = getopt(argc, argv, "1:Dt:i:o:a:2:")) != -1) {
        argTrue = true;
        

		switch (c){
            default:
            case '1': //if '-1' is present
            {
                *pName = optarg;
    
                break;

            }
            case '2': //if '-2' is present
            {
                *sPName = optarg;

                break;

            }
            case 'i': //if '-i' is present
            {
                *fName = optarg;

                break;

            }
            case 'o': //if '-o' is present
            {
                *oName = optarg;

                break;

            }
            case 'a': //if '-a- is present
            {
                *aName = optarg;

                break;
            }
            case 'D': //if '-D' is present
            {
                char cwd[128];
                cout << getcwd(cwd, sizeof(cwd)) << endl; //print out an array of chars that hold the values of the current directory
                
                break;

            }

        }


    }

    return argTrue;

}