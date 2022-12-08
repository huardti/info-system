#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "sem.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
    SEMAPHORE sem = sem_use(FALSE, "/tmp/lol", 42);

    printf("[Process B] PID %d sleeping 3 seconds...\n", getpid());
    sleep(3);

    puts("[Process B] Exiting");
    V(sem);

    return EXIT_SUCCESS;
}