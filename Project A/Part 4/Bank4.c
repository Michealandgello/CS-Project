#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int BOA = 1000; // Account A
int Wellsfarbo = 1000; // Account B
pthread_mutex_t locksmith = PTHREAD_MUTEX_INITIALIZER; // Mutex for Account A
pthread_mutex_t smithlock = PTHREAD_MUTEX_INITIALIZER; // Mutex for Account B

// Timeout for halted threads (in seconds)
#define timer 2

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

// Transfer money from Account A to Account B
void *A2B(void *arg) {
    int id = *((int *)arg); // Thread ID
    int amount = (rand() % 300) + 1; // Random transfer amount between $1 and $300

    // Retry loop for deadlock recovery
    while (1) { 
        // Lock Account A
        printf("Thread %2d: Attempting to lock Account A.\n", id);
        if (timeout(&locksmith, timer) != 0) {
            printf("Thread %2d: Failed to lock Account A (timeout). Retrying...\n", id);
            continue; // Retry
        }
        printf("Thread %2d: Locked Account A.\n", id);

        // Lock Account B
        printf("Thread %2d: Attempting to lock Account B.\n", id);
        if (timeout(&smithlock, timer) != 0) {
            printf("Thread %2d: Failed to lock Account B (timeout). Releasing Account A and retrying...\n", id);
            pthread_mutex_unlock(&locksmith); // Release Account A
            continue; // Retry
        }
        printf("Thread %2d: Locked Account B.\n", id);

        // Perform the transfer
        if (BOA >= amount) {
            BOA -= amount;
            Wellsfarbo += amount;
            printf("Thread %2d: Transferred $%d from Account A to Account B.\n", id, amount);
        } else {
            printf("Thread %2d: Failed to transfer $%d from Account A to Account B (insufficient funds).\n", id, amount);
        }

        // Release locks
        pthread_mutex_unlock(&smithlock);
        pthread_mutex_unlock(&locksmith);
        printf("Thread %2d: Released all locks.\n", id);

        break; // Exit the retry loop
    }
    return NULL;
}

// Transfer money from Account B to Account A
void *B2A(void *arg) {
    int id = *((int *)arg); // Thread ID
    int amount = (rand() % 100) + 1; // Random transfer amount between $1 and $100

    // Retry loop for deadlock recovery
    while (1) { 
        // Lock Account A first for "proper resource ordering"
        printf("Thread %2d: Attempting to lock Account A.\n", id);
        if (timeout(&locksmith, timer) != 0) {
            printf("Thread %2d: Failed to lock Account A (timeout). Retrying...\n", id);
            continue; // Retry
        }
        printf("Thread %2d: Locked Account A.\n", id);

        // Lock Account B
        printf("Thread %2d: Attempting to lock Account B.\n", id);
        if (timeout(&smithlock, timer) != 0) {
            printf("Thread %2d: Failed to lock Account B (timeout). Releasing Account A and retrying...\n", id);
            pthread_mutex_unlock(&locksmith); // Release Account A
            continue; // Retry
        }
        printf("Thread %2d: Locked Account B.\n", id);

        // Perform the transfer
        if (Wellsfarbo >= amount) {
            Wellsfarbo -= amount;
            BOA += amount;
            printf("Thread %2d: Transferred $%d from Account B to Account A.\n", id, amount);
        } else {
            printf("Thread %2d: Failed to transfer $%d from Account B to Account A (insufficient funds).\n", id, amount);
        }

        // Release locks
        pthread_mutex_unlock(&smithlock);
        pthread_mutex_unlock(&locksmith);
        printf("Thread %2d: Released all locks.\n", id);

        break; // Exit the retry loop
    }

    return NULL;
}

// Deadlock detection for A and B
void IMDEAD() {
    if (pthread_mutex_trylock(&locksmith)) {
        printf("Deadlock detected: Account A is locked.\n");
    } else {
        pthread_mutex_unlock(&locksmith);
    }
    if (pthread_mutex_trylock(&smithlock)) {
        printf("Deadlock detected: Account B is locked.\n");
    } else {
        pthread_mutex_unlock(&smithlock);
    }
}

int main() {
    srand(time(NULL)); // Random number generator
    int threadings = 10; // Number of threads
    pthread_t threads[threadings]; // Array to hold thread IDs
    int threadIDs[threadings]; // Array to hold thread numbers for identification

    printf("Starting balances:\n");
    printf("Account A: $%d\n", BOA);
    printf("Account B: $%d\n", Wellsfarbo);

    // Create threads for transfers
    for (int i = 0; i < threadings; i++) {
        threadIDs[i] = i + 1; // Assign thread IDs
        if (i % 2 == 0) {
            // Even threads transfer from A to B
            if (pthread_create(&threads[i], NULL, A2B, &threadIDs[i]) != 0) {
                fprintf(stderr, "Error creating thread %d\n", i);
                return 1;
            }
        } else {
            // Odd threads transfer from B to A
            if (pthread_create(&threads[i], NULL, B2A, &threadIDs[i]) != 0) {
                fprintf(stderr, "Error creating thread %d\n", i);
                return 1;
            }
        }
    }

    // Start deadlock detection
    sleep(timer); // Wait for threads to potentially deadlock
    IMDEAD();

    // Wait for all threads to finish
    for (int i = 0; i < threadings; i++) {
        pthread_join(threads[i], NULL);
    }

    // Finally done?
    printf("Final balances:\n");
    printf("Account A: $%d\n", BOA);
    printf("Account B: $%d\n", Wellsfarbo);

    return 0;
}