#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variable shared by threads
int count = 0;

// Mutex protects global variable. Only 1 thread can access at a time
// PTHREAD_MUTEX_INITIALIZER is faster than declaring and initializing
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Both threads will run this:
void* INTcrement(void* arg) {
    int num_increments = *(int*)arg;

    // From 0 to the randomly generated increments
    for (int i = 0; i < num_increments; i++) {
        // Acquire lock
        pthread_mutex_lock(&mutex);
        // Critical section
        count++;
        // Release lock
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Random number generator with time as its seed (without time, random number generated is same)
    srand(time(NULL));
    int max_rand = 999999;
    // Number of times each thread will increment the counter
    int inc_per_thread = rand() % (max_rand + 1);

    // Each thread will run the method
    pthread_create(&thread1, NULL, INTcrement, &inc_per_thread);
    pthread_create(&thread2, NULL, INTcrement, &inc_per_thread);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // The result should always be 2x the inc per thread
    printf("Numbers to be generated: %d\n", inc_per_thread);
    printf("Final counter value: %d\n", count);

    return 0;
}