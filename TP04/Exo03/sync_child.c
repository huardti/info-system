#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "sem.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
    SEMAPHORE sem = sem_create(TRUE, "", 0);
    sem_init(sem, (unsigned short[]){1});

    P(sem);
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        return EXIT_FAILURE;
    }
    else if(pid > 0) // Parent
    {
        printf("[Parent] PID %d created child PID %d\n", getpid(), pid);

        puts("[Parent] Waiting on child");
        P(sem);

        puts("[Parent] Exiting");
        V(sem);

        sem_delete(sem);
        return EXIT_SUCCESS;
    }
    else // Child
    {
        printf("[Child] PID %d created by parent PID %d\n", getpid(), getppid());

        puts("[Child] Sleeping 3 seconds...");
        sleep(3);

        puts("[Child] Exiting");
        V(sem);

        return EXIT_SUCCESS;
    }
}