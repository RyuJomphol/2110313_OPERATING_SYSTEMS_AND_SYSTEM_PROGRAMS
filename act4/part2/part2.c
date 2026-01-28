#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int tokenize(char *string, char *delimiters, char ***arrayOfTokens) { 
    char *token; 
    int numtokens; 
    int i; 

    string += strspn(string, delimiters); 
    if ((token = malloc(strlen(string) + 1)) == NULL) return -1; 
    strcpy(token, string); 
    numtokens = 0; 

    if (strtok(token, delimiters) != NULL) 

    for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++); 

    if ((*arrayOfTokens = malloc((numtokens+1)*sizeof(char *))) == NULL) { 
        free(token); 
        return -1; 
    } 

    if (numtokens == 0) free(token); 
    else { 
        strcpy(token, string); 
        (*arrayOfTokens)[0] = strtok(token, delimiters); 
        for (i = 1; i < numtokens; i++) 
        (*arrayOfTokens)[i] = strtok(NULL, delimiters); 
        (*arrayOfTokens)[numtokens] = NULL; 
    } 
    return numtokens; 
} 

int main() 
{ 
    int run = 1; 
    char buffer[256];
    char delim[] = " \t\n";
    char **args;
    int numtokens;
    pid_t pid;

    while(run) { 
        printf("mysh >"); 
        
        if (fgets(buffer, 256, stdin) == NULL) {
            break;
        }

        numtokens = tokenize(buffer, delim, &args);

        if (numtokens <= 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            run = 0;
        } 
        else {
            pid = fork();

            if (pid < 0) {
                perror("Fork failed");
            } 
            else if (pid == 0) {
                execvp(args[0], args);
                
                printf("Command not found: %s\n", args[0]);
                exit(1); 
            } 
            else {
                wait(NULL);
            }
        }
    } 
    return 0;
}