#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

void* thread_task(void* message) {
    char* msg = (char*)message;
    printf("%s\n", msg);
    pthread_exit(NULL);
}

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        pthread_t t1, t2;

        pthread_create(&t1, NULL, thread_task, "First thread from child process");
        pthread_create(&t2, NULL, thread_task, "Second thread from child process");

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        exit(0);
    }
    else {

        pthread_t t1, t2;

        pthread_create(&t1, NULL, thread_task, "First thread from parent process");
        pthread_create(&t2, NULL, thread_task, "Second thread from parent process");

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        wait(NULL);
    }

    return 0;
}