#include "sem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

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

SEMAPHORE sem_create(int private, char* filepath, int id)
{
    key_t semkey;

    if(private)
    {
        semkey = IPC_PRIVATE;
    }
    else
    {
        semkey = ftok(filepath, id);
        if(semkey == -1)
        {
            perror("ftok()");
            exit(EXIT_FAILURE);
        }
    }

    SEMAPHORE return_value = semget(semkey, 1, IPC_CREAT | 0666);
    if(return_value == -1)
    {
        perror("semget()");
        exit(EXIT_FAILURE);
    }

    return return_value;
}
SEMAPHORE sem_use(int private, char* filepath, int id)
{
    key_t semkey;

    if(private)
    {
        semkey = IPC_PRIVATE;
    }
    else
    {
        semkey = ftok(filepath, id);
        if(semkey == -1)
        {
            perror("ftok()");
            exit(EXIT_FAILURE);
        }
    }

    SEMAPHORE return_value = semget(semkey, 1, 0666);
    if(return_value == -1)
    {
        perror("semget()");
        exit(EXIT_FAILURE);
    }

    return return_value;
}
void sem_delete(SEMAPHORE sem)
{
    if(semctl(sem, 0, IPC_RMID) != 0)
    {
        perror("semctl([...], IPC_RMID)");
        exit(EXIT_FAILURE);
    }
}
void sem_init(SEMAPHORE sem, unsigned short* intarray)
{
    if(semctl(sem, 0, SETALL, intarray) != 0)
    {
        perror("semctl([...], SETALL)");
        exit(EXIT_FAILURE);
    }
}