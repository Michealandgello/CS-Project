#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


// Two bank accounts with their own mutexes
int BOA = 1000; // Account A
int Wellsfarbo = 1000; // Account B
pthread_mutex_t locksmith = PTHREAD_MUTEX_INITIALIZER; // Mutex for Account A
pthread_mutex_t smithlock = PTHREAD_MUTEX_INITIALIZER; // Mutex for Account B

// Transfer money from Account A to Account B
void *A2B(void *arg) {
    int id = *((int *)arg); // Thread ID
    int amount = (rand() % 100) + 1; // Random transfer amount between $1 and $100

    // Lock Account A, then Account B
    printf("Thread %2d: Locking Account A.\n", id);
    pthread_mutex_lock(&locksmith);
    printf("Thread %2d: Locked Account A.\n", id);

    sleep(1); // Simulate some work

    printf("Thread %2d: Locking Account B\n", id);
    pthread_mutex_lock(&smithlock);
    printf("Thread %2d: Locked Account B.\n", id);

    // Transfer between accounts
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

    return NULL;
}

// Transfer money from Account B to Account A
void *B2A(void *arg) {
    int id = *((int *)arg); // Thread ID
    int amount = (rand() % 100) + 1; // Random transfer amount between $1 and $100

    // Lock Account B, then Account A
    printf("Thread %2d: Locking Account B\n", id);
    pthread_mutex_lock(&smithlock);
    printf("Thread %2d: Locked Account B\n", id);

    sleep(1); // Sleeping on the job

    printf("Thread %2d: Locking Account A.\n", id);
    pthread_mutex_lock(&locksmith);
    printf("Thread %2d: Locked Account A.\n", id);

    // Perform the transfer
    if (Wellsfarbo >= amount) {
        Wellsfarbo -= amount;
        BOA += amount;
        printf("Thread %2d: Transferred $%d from Account B to Account A.\n", id, amount);
    } else {
        printf("Thread %2d: Failed to transfer $%d from Account B to Account A (insufficient funds).\n", id, amount);
    }

    // Release locks
    pthread_mutex_unlock(&locksmith);
    pthread_mutex_unlock(&smithlock);

    printf("Thread %2d: Released all locks.\n", id);
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
    int threadings = 2; // Changeable amount of threads
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

    // Simulate deadlock detection
    sleep(2); // Wait for threads to potentially deadlock
    IMDEAD();

    // Wait for all threads to finish
    for (int i = 0; i < threadings; i++) {
        pthread_join(threads[i], NULL);
    }

    // We will NEVER make it here
    printf("Final balances:\n");
    printf("Account A: $%d\n", BOA);
    printf("Account B: $%d\n", Wellsfarbo);

    return 0;
}