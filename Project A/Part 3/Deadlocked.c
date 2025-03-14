#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Two mutex locks
pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

// Detects deadlocks
void IMDEAD() {
    if (pthread_mutex_trylock(&first_mutex)) {
        printf("First mutex is locked!\n");
    } else {
        pthread_mutex_unlock(&first_mutex); // Unlock if acquired
    }
    if (pthread_mutex_trylock(&second_mutex)) {
        printf("Second mutex is locked!\n");
    } else {
        pthread_mutex_unlock(&second_mutex); // Unlock if acquired
    }
}

// Thread one runs this
void *do_some_work1(void *param) {
    printf("Thread 1: Locking first mutex\n");
    pthread_mutex_lock(&first_mutex); // Lock first_mutex
    printf("Thread 1: Locked first mutex\n");

    // Simulate some work
    sleep(1);

    printf("Thread 1: Locking second mutex\n");
    pthread_mutex_lock(&second_mutex); // Lock second_mutex
    printf("Thread 1: Locked second_mutex.\n");

    // Simulate more work
    printf("Thread 1: I'm tired...\n");
    sleep(1);

    // Release locks
    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);
    printf("Thread 1: Released all locks\n");

    pthread_exit(0);
}

// Thread two runs this
void *do_some_work2(void *param) {
    printf("Thread 2: Attempting to lock second_mutex.\n");
    pthread_mutex_lock(&second_mutex); // Lock second mutex
    printf("Thread 2: Locked second_mutex.\n");

    // Simulate some work
    int k = 0;
    for (int i = 0; i <= 100; i++) {
        k += i;
    }
    printf("Thread 2 calculation: %d\n", k);

    printf("Thread 2: Attempting to lock first_mutex.\n");
    pthread_mutex_lock(&first_mutex); // Lock first mutex
    printf("Thread 2: Locked first_mutex.\n");

    // Simulate more work
    k = 0;
    for (int i = 0; i <= 100; i++) {
        k += i;
    }
    printf("Thread 2 calculation: %d\n", k);

    // Release locks
    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);

    printf("Thread 2: Released all locks\n");
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

    // Simulate deadlock detection
    sleep(2); // Wait for threads to potentially deadlock
    IMDEAD();

    // Wait for the threads to finish (this will hang if deadlock occurs)
    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    // Destroy the mutex locks
    pthread_mutex_destroy(&first_mutex);
    pthread_mutex_destroy(&second_mutex);

    printf("Did we make it?\n");
    return 0;
}