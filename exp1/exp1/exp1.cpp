#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
void child_process_logic() {
    printf("Child process started with PID: %d\n", getpid());

    sleep(rand() % 3 + 1);  // delay random time
    
    printf("Child process finished its task.\n");
    execl("/bin/ls", "ls", NULL); 
}

void parent_process_logic(pid_t child_pid) {
    printf("Parent process started with PID: %d\n", getpid());
    
    // parent process job
    printf("Parent is waiting for the child process to complete...\n");
    
    int status;
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status)) {
        printf("Child process exited with status: %d\n", WEXITSTATUS(status));
    }

    printf("Parent process finished.\n");
}

int main() {
    srand(time(NULL));
    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Fork failed! Error: %s\n", strerror(errno));
        exit(1);
    }

    if (pid == 0) {
        child_process_logic();
    } else {
        wait(NULL);
        parent_process_logic(pid);
    }

    return 0;
}
