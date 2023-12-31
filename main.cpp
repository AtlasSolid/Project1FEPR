//Noah Wilson and Katie Wilde

#include <iostream>
#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>

using namespace std;

int main(int argc, char * argv[]) {


    //checks the command line args
    bool HandleOptions(int argc, char ** argv, char**, char**, char**, char**, char**, char**);
    void printError();


    char * pathName = 0;
    char * secondPathName = 0;
    char * fileName = 0;
    char * oFileName = 0;
    char * aFileName = 0;
    char * dirName = 0;

    //if no arguments provided, print error message, do not fork, do not pipe, do not exec, end process here
    if (HandleOptions(argc, argv, &pathName, &secondPathName, &fileName, &oFileName, &aFileName, &dirName) == false) {
		printError();
		return 1;
	}

    int p[2]; //array of two pipe file descriptors

    char * myargv[2]; //array of char pointers

    if (secondPathName == 0) //if '-2' is not in the arguments
    {
        pid_t rc = fork(); //parent forks child 1

        if (rc < 0) // if the fork failed, print error and exit
        { 
            fprintf(stderr, "fork failed\n");

            exit(1); 
        }
        else if (rc == 0) //if fork was successful
        {

            myargv[0] = strdup(pathName); //program to execute, either ls or wc depending on cmd line args
            myargv[1] = 0; //file that ls or wc would be reading from

            if (fileName != 0) //if a file name was passed
            {
                int fd = open(fileName, S_IRUSR); //wiring -i's file path to the standard input
                if (fd > 0) //if open() was successful
                {
                    close(STDIN_FILENO);
                    dup(fd);
                    close(fd);
                }
                else
                {
                    fprintf(stderr, "Invalid file\n");
                    exit(1);
                }
            }
            
            execvp(myargv[0], myargv); //executes ls or wc, also checks if the path leads to an executable file

            cout << "Failed exec call" << endl; //this line shouldn't execute unless execvp() screwed up
        }
        else //parent goes down this path
        {
            int wstatus;

            waitpid(rc, &wstatus, 0); //wait until c1 is complete
            printf("Child 1: %d returns: %d\n", rc, WEXITSTATUS(wstatus));
        }

    }
    else
    {
        //int* rcP = nullptr;
        //int* rc2P = nullptr;

        pid_t rc = 0;
        pid_t rc2 = 0;

        int pFail = pipe(p);

        if (pFail < 0) //if pipe failed, print error and exit
        {
            fprintf(stderr, "pipe failed\n");

            exit(1); 
        }
        else
        {
            rc = fork(); //parent forks child 1

            if (rc < 0)
            { 
                fprintf(stderr, "child 1 fork failed\n");

                exit(1); 
            }
            else if (rc == 0)
            {

                close(p[0]); //not reading from the pipe, so close read side
                close(STDOUT_FILENO); //close STDOUT to make it available

                dup(p[1]); //dup write side of the pipe

                close(p[1]); //close original write side of pipe, connection still in File Descriptor 1

                myargv[0] = strdup(pathName);
                myargv[1] = 0;

                if (fileName != 0) //if -i case present, wire -i file into standard input, first in the sequence
                {
                    int fd = open(fileName, O_SYNC, S_IRUSR); //read mode
                    if (fd > 0)
                    {
                        close(STDIN_FILENO);
                        dup(fd);
                        close(fd);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid file\n");
                        exit(1);
                    }
                }
                execvp(myargv[0], myargv);

                cout << "Failed exec call" << endl;
            }
            else //parent goes down this path
            {  


            }

            rc2 = fork(); //parent forks child 2

            if (rc2 < 0)
            { 
                fprintf(stderr, "child 2 fork failed\n");

                exit(1); 
            }
            else if (rc2 == 0)
            {

                close(p[1]); //close write side of the pipe
                close(STDIN_FILENO); //close STDIN to make it available

                dup(p[0]); //dup read side of the pipe

                close(p[0]); //close original read side of pipe, connection still in File Descriptor 0

                myargv[0] = strdup(secondPathName);
                myargv[1] = 0;

                if (oFileName != 0) //if -o case present, wire -o file into standard output, last in the sequence
                {
                    int fd2 = open(oFileName, O_WRONLY, S_IRUSR); //overwrite mode
                    if (fd2 > 0)
                    {
                        close(STDOUT_FILENO);
                        dup(fd2);
                        close(fd2);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid file\n");
                        exit(1);
                    }
                }
                else if (aFileName != 0) //if -a case present, wire -a file into standard output, last in the sequence
                {
                    int fd2 = open(aFileName, O_APPEND | O_WRONLY, S_IRUSR); //append mode
                    if (fd2 > 0)
                    {
                        close(STDOUT_FILENO);
                        dup(fd2);
                        close(fd2);
                    }
                    else
                    {
                        fprintf(stderr, "Invalid file\n");
                        exit(1);
                    } 
                }

                execvp(myargv[0], myargv);

                cout << "Failed exec call" << endl;
            }
            else
            {

            }

            int wstatus;

            close(p[0]);
            close(p[1]); //closing read and write sides of pipe
            waitpid(rc2, &wstatus, 0); //wait until c1 is complete
            printf("Child 2: %d returns: %d\n", rc2, WEXITSTATUS(wstatus));
            waitpid(rc, &wstatus, 0); //wait until c2 is complete
            printf("Child 1: %d returns: %d\n", rc, WEXITSTATUS(wstatus));

        }
    

    }



    return 0;

}

void printError()
{
    cout << "Missing required command line option -1" << endl;


}

bool HandleOptions(int argc, char ** argv, char** pName, char** sPName, char** fName, char** oName, char** aName, char** dName) {
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
            case 'a': //if '-a' is present
            {
                *aName = optarg;

                break;
            }
            case 't': //if '-t' is present
            {
                *dName = optarg;

                chdir(*dName);

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