#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

/* Taken from textbook figure 3.22 and upgraded to implement
   bidirectional (two way) communication! */ 

int main(void) {
    char parent_msg[BUFFER_SIZE] = "Hello from parent";
    char child_msg[BUFFER_SIZE] = "Hello from child";
    char read_buffer[BUFFER_SIZE];

    // Two pipes: one for parent-to-child communication, one for child-to-parent
    int fd1[2]; // Parent writes to child
    int fd2[2]; // Child writes to parent

    pid_t pid;

    // Create the first pipe
    if (pipe(fd1)) {
        fprintf(stderr, "Pipe 1 failed\n");
        return 1;
    }

    // Create the second pipe
    if (pipe(fd2)) {
        fprintf(stderr, "Pipe 2 failed\n");
        return 1;
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) { // Error occurred
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) { // Parent process
        // Close unused ends of the pipes
        close(fd1[READ_END]); // Close read end of pipe 1
        close(fd2[WRITE_END]); // Close write end of pipe 2

        // Write to the child
        printf("Parent: Writing message to child: %s\n", parent_msg);
        write(fd1[WRITE_END], parent_msg, strlen(parent_msg) + 1);

        // Close the write end of pipe 1
        close(fd1[WRITE_END]);

        // Read from the child
        read(fd2[READ_END], read_buffer, BUFFER_SIZE);
        printf("Parent: Received message from child: %s\n", read_buffer);

        // Close the read end of pipe 2
        close(fd2[READ_END]);
    } else { // Child process
        // Close unused ends of the pipes
        close(fd1[WRITE_END]); // Close write end of pipe 1
        close(fd2[READ_END]); // Close read end of pipe 2

        // Read from the parent
        read(fd1[READ_END], read_buffer, BUFFER_SIZE);
        printf("Child: Received message from parent: %s\n", read_buffer);

        // Close the read end of pipe 1
        close(fd1[READ_END]);

        // Write to the parent
        printf("Child: Writing message to parent: %s\n", child_msg);
        write(fd2[WRITE_END], child_msg, strlen(child_msg) + 1);

        // Close the write end of pipe 2
        close(fd2[WRITE_END]);
    }

    return 0;
}