//
//  processes.cpp
//  CSS430
//


#include <stdio.h>
#include <stdlib.h>     //exit
#include <unistd.h>     //fork, pipe
#include <sys/wait.h>   //wait function
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    enum {READ, WRITE};
    pid_t pid1, pid2, pid3;
    int pipeFD1[2],pipeFD2[2];
    
    //check if one argument is provided
    if(argc < 2)
    {
        perror("Error in Argument: required 1");
        exit(EXIT_FAILURE);
    }
    
    //check if the pipe is correctly created
    if (pipe(pipeFD1) < 0)
    {
        perror("Error in creating pipe 1");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipeFD2) < 0)
    {
        perror("Error in creating pipe 2");
        exit(EXIT_FAILURE);
    }
    
    //creating the fork process
    pid1 = fork();
    //check if the fold process is good
    if (pid1 < 0)
    {
        perror("Error during the first fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid1 == 0)  //Child1
    {
        //creating the fork process
        pid2 = fork();
        if (pid2 < 0)
        {
            perror("Error during the second fork");
            exit(EXIT_FAILURE);
        }
        else if (pid2 == 0)//Child2
        {
            //creating the fork process
            pid3 = fork();
            if (pid3 < 0)
            {
                perror("Error during the third fork");
                exit(EXIT_FAILURE);
            }
            else if (pid3 == 0)// child3
            {
                //child3
                close(pipeFD1[WRITE]);
                close(pipeFD2[WRITE]);
                close(pipeFD1[READ]);
                dup2(pipeFD2[READ], 0);
                execlp("wc", "wc", "-l", NULL);
            }
            else
            {
                //child2
                close(pipeFD1[WRITE]);
                dup2(pipeFD2[WRITE],1);
                dup2(pipeFD1[READ],0);
                close(pipeFD2[READ]);
                execlp("grep", "grep", argv[1], NULL);
            }
        }
        else
        {
        //child1
        dup2(pipeFD1[WRITE],1);
        close(pipeFD2[WRITE]);
        close(pipeFD1[READ]);
        close(pipeFD2[READ]);
        execlp("ps", "ps", "-A", NULL);
        }
    }
    else
    {
        //parent
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}
