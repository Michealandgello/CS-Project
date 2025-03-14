#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int moolah = 1000; // Starting balance

// Function for deposits
void *addMoney(void *arg) {
    int id = *((int *)arg); // Thread ID
    int deposit = (rand() % 100) + 1; // Random deposit amount between $1 and $100
    moolah += deposit; // Update the balance (no thread safety)
    printf("Thread %2d deposited $%d.  Balance is now $%4d.\n", id, deposit, moolah);
    return NULL;
}

// Function to simulate a withdrawal
void *takeMoney(void *arg) {
    int id = *((int *)arg); // Thread ID
    int withdrawal = (rand() % 100) + 1; // Random withdrawal amount between $1 and $100
    if (moolah >= withdrawal) { // Our bank does not allow loans...
        moolah -= withdrawal; // Update the balance (no thread safety)
        printf("Thread %2d withdrew  $%d.  Balance is now $%4d.\n", id, withdrawal, moolah);
    } else {
        printf("Thread %2d failed to withdraw $%d. Insufficient funds!\n", id, withdrawal);
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Random number generator
    int threadings = 10; // Changeable amount of threads (keep it even)
    pthread_t threads[threadings]; // Array to hold thread IDs
    int threadIDs[threadings]; // Array to hold thread numbers for identification

    printf("Starting bank account balance: $%d\n", moolah);

    // Half of threads are deposit threads and rest are withdrawal threads
    for (int i = 0; i < threadings / 2; i++) {
        threadIDs[i] = i + 1; // Add 1 so thread ID will not start at 0
        if (pthread_create(&threads[i], NULL, addMoney, &threadIDs[i]) != 0) { // Thread only returns 0 if successful
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    for (int j = threadings / 2; j < threadings; j++) {
        threadIDs[j] = j + 1;
        if (pthread_create(&threads[j], NULL, takeMoney, &threadIDs[j]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", j);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int k = 0; k < threadings; k++) {
        pthread_join(threads[k], NULL);
    }

    // Works most of the time?
    printf("Final bank account balance: $%d\n", moolah);
    return 0;
}