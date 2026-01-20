#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    printf("I am the parent process. My PID is %d\n", getpid());
    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid1 == 0) {
        printf("I am the child process. My PID is %d and my parent's PID is %d\n", getpid(), getppid());
        pid2 = fork();

        if (pid2 < 0) {
            fprintf(stderr, "Fork Failed");
            return 1;
        }
        else if (pid2 == 0) {
            printf("I am the grandchild process. My PID is %d and my parent's PID is %d\n", getpid(), getppid());
        }
        else {
            wait(NULL);
        }
    }
    else {
        wait(NULL);
    }

    return 0;
}