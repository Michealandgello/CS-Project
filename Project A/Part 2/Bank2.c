#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int moolah = 1000; // Starting balance
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function for deposits
void *addMoney(void *arg) {
    int id = *((int *)arg); // Thread ID
    int deposit = (rand() % 100) + 1; // Random deposit amount between $1 and $100
    pthread_mutex_lock(&lock); // Lock the mutex
    moolah += deposit; // Update the balance
    printf("Thread %2d deposited $%d.  Balance is now $%4d.\n", id, deposit, moolah);
    pthread_mutex_unlock(&lock); // Unlock the mutex
    return NULL;
}

// Function to simulate a withdrawl
void *takeMoney(void *arg) {
    int id = *((int *)arg); // Thread ID
    int withdrawal = (rand() % 100) + 1; // Random withdrawal amount between $1 and $100
    pthread_mutex_lock(&lock);
    if (moolah >= withdrawal) { // Our bank does not allow loans...
        moolah -= withdrawal;
        printf("Thread %2d withdrew  $%d.  Balance is now $%4d.\n", id, withdrawal, moolah);
    } else {
        printf("Thread %2d failed to withdraw $%d. Insufficient funds!\n", id, withdrawal);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    srand(time(NULL)); // Random number generator
    int threadings = 30; // Changeable amount of threads (keep it even)
    pthread_t threads[threadings]; // Array to hold thread IDs
    int threadIDs[threadings]; // Array to hold thread numbers for identification

    printf("Starting bank account balance: $%d\n", moolah);

      // Half of threads are deposit threads and rest are withdrawal threads
      for (int i = 0; i < threadings/2; i++) {
        threadIDs[i] = i + 1; // Add 1 so thread ID will not start at 0
        if (pthread_create(&threads[i], NULL, addMoney, &threadIDs[i]) != 0) { // Thread only returns 0 if succesful
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    for (int j = threadings/2; j < threadings; j++) {
        threadIDs[j] = j + 1;
        if (pthread_create(&threads[j], NULL, takeMoney, &threadIDs[j]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", j);
            return 1;
        }
    }

    // Wait for all threads to finish once again
    for (int k = 0; k < threadings; k++) {
        pthread_join(threads[k], NULL);
    }

    printf("Final bank account balance: $%d\n", moolah);

    return 0;
}