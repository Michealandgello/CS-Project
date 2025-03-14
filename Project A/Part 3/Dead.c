#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Borrowed from psudocode in textbook

// Two mutex locks
pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

// Thread one runs this
void *do_some_work1(void *param) {
    printf("Thread 1: Locking first mutex\n");
    pthread_mutex_lock(&first_mutex); // Lock first_mutex
    printf("Thread 1: Locked first mutex\n");

    // Work attempt 1
    sleep(1);

    printf("Thread 1: Locking second mutex\n");
    pthread_mutex_lock(&second_mutex); // Lock second_mutex
    printf("Thread 1: Locked second_mutex.\n");

    // Work attempt 2
    printf("Thread 1: I'm tired...\n");
    sleep(1);

    // Release locks
    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);

    printf("Released all locks for thread 1\n");
    pthread_exit(0);
}

// Thread two runs this
void *do_some_work2(void *param) {
    printf("Thread 2: Locking second mutex.\n");
    pthread_mutex_lock(&second_mutex); // Lock second mutex
    printf("Thread 2: Locked second mutex.\n");

    // Do some work 1
    int k = 0;
    for (int i = 0; i < 100; i++) {
        k =+ i;
    }
    printf("Thread 2 calculation: %d\n",k);

    printf("Thread 2: Locking first mutex.\n");
    pthread_mutex_lock(&first_mutex); // Lock first mutex
    printf("Thread 2: Locked first mutex.\n");

    // Do some work 2
    k = 0;
    for (int i = 0; i < 100; i++) {
        k =+ i;
    }
    printf("Thread 2 calculation: %d\n",k);

    // Release locks
    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);

    printf("Released all locks for thread 2\n");
    pthread_exit(0);
}

int main() {
    pthread_t thread_one, thread_two;

    // Initialize the mutex locks
    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);

    // Create two threads
    pthread_create(&thread_one, NULL, do_some_work1, NULL);
    pthread_create(&thread_two, NULL, do_some_work2, NULL);

    // Wait for the threads to finish
    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    // Destroy the mutex locks
    pthread_mutex_destroy(&first_mutex);
    pthread_mutex_destroy(&second_mutex);

    printf("Did we make it?\n");
    return 0;
}