#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "sem.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
    SEMAPHORE sem = sem_create(FALSE, "/tmp/lol", 42);
    sem_init(sem, (unsigned short[]){0});

    printf("[Process A] PID %d waiting on Process B...\n", getpid());
    P(sem);

    puts("[Process A] Exiting");
    V(sem);

    sem_delete(sem);
    return EXIT_SUCCESS;
}