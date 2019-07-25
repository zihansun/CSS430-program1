#include <stdio.h>
#include <stdlib.h>     //exit
#include <unistd.h>     //fork, pipe
#include <sys/wait.h>   //wait function
#include <iostream>
using namespace std;

int main(int arg, char *argv[]){
    // enum {0, 1}; // user "0" and "1" instead of nurmerical number 
    pid_t pid1, pid2, pid3; // init pid
    int pipeFD1[2],pipeFD2[2];  // init two file descripter
    
    // check if pipes are created ok
    if(pipe(pipeFD1)<0 || pipe(pipeFD2)<0){
        perror("Error in creating pipe");
        exit(EXIT_FAILURE);
    }

    // check for argument 
    if(arg < 2){ 
        perror("An argument is expected");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();  // create fork process
    if(pid1 < 0){
        perror("Error during fork");      
        exit(EXIT_FAILURE);
    }   
    if(pid1 == 0){   // child 
        pid2 = fork();
        if(pid2 < 0){
            perror("Error during fork");      
            exit(EXIT_FAILURE);
        }
        if(pid2 == 0){
            pid3 = fork();
            if(pid3 < 0){
                perror("Error during fork");
                exit(EXIT_FAILURE);
            }
            if(pid3 == 0){
                close(pipeFD1[0]);  // close FD1 READ
                close(pipeFD1[1]); // close FD1 WRITE
                close(pipeFD2[1]); // close FD2 WRITE
                dup2(pipeFD2[1],1); // READ to FD2
                int check = execlp("wc", "wc", "-l", (char*)0);
                if(check == -1){
                    perror("Error on executig command ");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                close(pipeFD1[1]);
                dup2(pipeFD1[0],0); // open read for FD1
                close(pipeFD2[0]);  
                dup2(pipeFD2[1],1); // open write for FD2
                wait(NULL);
                int check = execlp("greap","greap",argv[1],(char*)0);
                if(check == -1){
                    perror("Error on executing command");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else{
            dup2(pipeFD1[1],1);
            close(pipeFD1[0]);
            close(pipeFD2[0]);
            close(pipeFD2[1]);
            wait(NULL);
            // int check = execlp("wc","wc","-l",(char*)0);
            int check = execlp("ps","ps","-A",(char*)0);
            if(check == -1){
                perror("Error on executing command");
                exit(EXIT_FAILURE);
            }
        }
        
    }
    else{
        wait(NULL);
    }
return 0;
}
