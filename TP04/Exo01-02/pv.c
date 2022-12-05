#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define TRUE 1
#define FALSE 0


int main(void)
{
    // SEMAPHORE sem = sem_create(TRUE, "", 0);
    SEMAPHORE sem = sem_create(FALSE, "/tmp/lol", 42);

    sem_init(sem, (unsigned short[]){1});

    puts("Semaphore created.");

    printf("%d\n", P(sem));
    printf("%d\n", V(sem));
    printf("%d\n", P(sem));

    sem_delete(sem);

    return EXIT_SUCCESS;
}