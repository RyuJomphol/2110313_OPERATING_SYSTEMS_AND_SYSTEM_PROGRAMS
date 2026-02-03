#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* print_thread_id(void* arg) {
    int id = *((int*)arg);
    
    printf("This is thread %d\n", id);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[10];
    
    int thread_ids[10];

    for (int i = 0; i < 10; i++) {
        thread_ids[i] = i + 1;
        status = pthread_create(&threads[i], NULL, print_thread_id, (void*)&thread_ids[i]);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}