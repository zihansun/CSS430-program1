//
//  processes.cpp
//  CSS430
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

    // check if pipes are created ok
    if(pipe(pipeFD1)<0 || pipe(pipeFD2)<0){
        perror("Error in creating pipe");
        exit(EXIT_FAILURE);
    }
    
    //check if one argument is provided
    if(argc < 2)
    {
        perror("Error in Argument: required 1");
        exit(EXIT_FAILURE);
    }

    //creating the fork process
    pid1 = fork();
    //check if the fold process is good
    if (pid1 < 0)
    {
        perror("Error during fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid1 == 0)  //Child1
    {
        //creating the fork process
        pid2 = fork();
        if (pid2 < 0)
        {
            perror("Error during fork");
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
                int check = execlp("wc", "wc", "-l", NULL);
                if(check == -1){
                perror("Error on executing command");
                exit(EXIT_FAILURE);
            }
            }
            else
            {
                //child2
                close(pipeFD1[WRITE]);
                dup2(pipeFD2[WRITE],1);
                dup2(pipeFD1[READ],0);
                close(pipeFD2[READ]);
                int check = execlp("grep", "grep", argv[1], NULL);
                if(check == -1){
                perror("Error on executing command");
                exit(EXIT_FAILURE);
            }
            }
        }
        else
        {
        //child1
        dup2(pipeFD1[WRITE],1);
        close(pipeFD2[WRITE]);
        close(pipeFD1[READ]);
        close(pipeFD2[READ]);
        int check = execlp("ps", "ps", "-A", NULL);
        if(check == -1){
                perror("Error on executing command");
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        //parent
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}
