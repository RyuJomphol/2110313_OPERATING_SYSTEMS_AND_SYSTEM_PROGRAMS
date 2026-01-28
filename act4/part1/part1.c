#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{ 
    pid_t pid; 

    if (argc < 2) {
        printf("Error: Please provide a command to execute.\n");
        printf("Usage: %s <command> [options]\n", argv[0]);
        exit(1);
    }

    pid = fork(); 

    if (pid < 0) { 
        perror("Error : cannot fork"); 
        exit(1); 
    } 
    else if (pid == 0) { 
        execvp(argv[1], &argv[1]);
        
        perror("Error executing command");
        exit(1);
    } 
    else { 
        wait(NULL); 
        return(0); 
    } 
}