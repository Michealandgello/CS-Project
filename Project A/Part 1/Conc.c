#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // Influences sleep

// Thing to be ran by first thread
void *threadcarefully1(void* arg) {
    printf("Thread 1, Operation 1\n");
    // Thread operation
    for(int i = 0; i < 6; i++) {
        // Prints increased count, then sleeps
        printf("Thread 1: %d\n", i);
        sleep(1);
    }
    return NULL;
}

// Thing to be ran by second thread (same thing)
void *threadcarefully2(void* arg) {
    printf("Thread 2, Operation 2\n");
    for(int i = 0; i < 6; i++) {
        printf("Thread 2: %d\n", i);
        sleep(1);
    }
    return NULL;
}

void main() {
    pthread_t thread1, thread2;

    // VERY simple thread creation. No attribute or argument var
    pthread_create(&thread1, NULL, threadcarefully1, NULL);
    pthread_create(&thread2, NULL, threadcarefully2, NULL);

    // Wait for threads to finish. Don't need return value
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Done!\n");
}