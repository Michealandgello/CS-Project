#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

    /* Textboox code, but improved slightly to handle errors*/

int sum; /* This data is shared by the thread(s) */

/* Thread function to be executed */
void *runner(void *param);

/* In C, argc is number of arguments. First argument is argv[0] always */
/* argv[] is a string array that holds command-line arguments argv[0] is program name */

int main(int argc, char *argv[]) {

    // Arguments = Program name + command line argument = 2
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return -1;
    }

    // atoi takes command arg string and converts it to int
    // If a negative number is run, sum will always be 0
    if (atoi(argv[1]) < 0) {
        fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
        return -1;
    }

    pthread_t tid; /* The thread identifier */
    pthread_attr_t attr; /* Set of thread attributes */

    /* Set the default attributes of the thread */
    pthread_attr_init(&attr);
    /* Create the thread */
    pthread_create(&tid, &attr, runner, argv[1]); // argv[1] Contains command line arg
    /* Wait for the thread to exit */
    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
    return 0;
}

/* The thread will execute in this function */
void *runner(void *param) {
    int i, upper = atoi((char *)param);
    sum = 0;
    for (i = 1; i <= upper; i++) {
        sum += i;
    }
    pthread_exit(0);
}
