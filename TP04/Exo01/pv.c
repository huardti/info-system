#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

typedef int SEMAPHORE;

int P(SEMAPHORE sem)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;

    return semop(sem, &sb, 1);
}
int V(SEMAPHORE sem)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;

    return semop(sem, &sb, 1);
}

int main(void)
{
    return EXIT_SUCCESS;
}