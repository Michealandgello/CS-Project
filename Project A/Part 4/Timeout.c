#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Two mutex locks
pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

// Timeout for halted threads (in seconds)
#define timer 5

// Function to attempt locking a mutex with a timeout
int timeout(pthread_mutex_t *mutex, int timeout_seconds) {
    time_t start_time = time(NULL); // Current time
    while (time(NULL) - start_time < timeout_seconds) { // While x amount of seconds has not passed
        if (pthread_mutex_trylock(mutex) == 0) {
            return 0; // Lock acquired successfully
        }
        sleep(1); // Wait 1 second then try again
    }
    return -1; // Deadlock confirmed
}

// Thread one runs this
void *do_some_work1(void *param) {
    printf("Thread 1: Attempting to lock first mutex\n");
    pthread_mutex_lock(&first_mutex); // Lock first mutex
    printf("Thread 1: Locked first mutex\n");

    // Simulate some work
    sleep(1);

    printf("Thread 1: Attempting to lock second mutex\n");
    if (timeout(&second_mutex, timer) == 0) {
        printf("Thread 1: Locked second mutex.\n");

        // Simulate more work
        printf("Thread 1: I'm tired...\n");
        sleep(1);

        // Release locks
        pthread_mutex_unlock(&second_mutex);
        pthread_mutex_unlock(&first_mutex);

        printf("Thread 1: Released all locks\n");
    } else { 
        printf("Thread 1: Deadlock detected! Releasing first mutex and retrying...\n");
        pthread_mutex_unlock(&first_mutex); // Release the first lock
        sleep(1); // Wait a sec
        do_some_work1(param); // Start over
    }
    pthread_exit(0);
}

// Thread two runs this
void *do_some_work2(void *param) {
    printf("Thread 2: Attempting to lock second mutex.\n");
    pthread_mutex_lock(&second_mutex); // Lock second mutex
    printf("Thread 2: Locked second mutex.\n");

    // Simulate some work
    int k = 0;
    for (int i = 0; i <= 100; i++) {
        k += i;
    }
    printf("Thread 2 calculation: %d\n", k);

    printf("Thread 2: Attempting to lock first mutex.\n");
    if (timeout(&first_mutex, timer) == 0) {
        printf("Thread 2: Locked first mutex.\n");

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
    } else {
        printf("Thread 2: Deadlock detected! Releasing second mutex and retrying...\n");
        pthread_mutex_unlock(&second_mutex);
        sleep(1);
        do_some_work2(param);
    }

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