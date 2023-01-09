#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "sem.h"
#include <signal.h>

#define TRUE    1
#define FALSE   0

typedef int SHMID;

void SIGUSR1_handler(int signum);

int main(void)
{
    SEMAPHORE sem = sem_create(FALSE, "/tmp/lol", 42);
    sem_init(sem, (unsigned short[]){1});

    SHMID shm = shmget(ftok("/tmp/lol", 42), 1000, IPC_CREAT | 0666);
    if(shm <= -1)
    {
        perror("shmget()");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR1, SIGUSR1_handler);

    pid_t pid;
    pid_t children_pids[2] = {0};

    for(int child = 0 ; child < 2 ; ++child)
    {
        pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0) // Parent
        {
            printf("[Parent %d]\tCreated child PID %d\n", getpid(), pid);
            children_pids[child] = pid;
        }
        else // Child
        {
            printf("[Child %d]\tCreated by parent PID %d\n", getpid(), getppid());
            void* shmptr = shmat(shm, NULL, 0);
            if(shmptr == (void*)-1)
            {
                perror("shmat()");
                exit(EXIT_FAILURE);
            }

            for(;;)
            {
                printf("[Child %d] Waiting...\n", getpid());
                pause();
                signal(SIGUSR1, SIGUSR1_handler);
                P(sem);

                printf("%s\n", (char*)shmptr);

                V(sem);
                kill(getppid(), SIGUSR1);
            }

            printf("[Child %d] Bye !\n", getpid());
            return EXIT_SUCCESS;
        }
    }

    void* shmptr = shmat(shm, NULL, 0);
    if(shmptr == (void*)-1)
    {
        perror("shmat()");
        exit(EXIT_FAILURE);
    }

    int current_child = 0;
    for(int i = 0 ; i < 10 ; ++i)
    {
        P(sem);
        sleep(1);

        sprintf((char*)shmptr, "Pouet%02d", i);
        printf("[Parent %d] Wrote stuff\n", getpid());

        V(sem);
        kill(children_pids[current_child], SIGUSR1);
        pause();
        signal(SIGUSR1, SIGUSR1_handler);

        current_child ^= 1;
    }


    P(sem);
    sem_delete(sem);
    shmctl(shm, IPC_RMID, NULL);
    kill(pid, SIGKILL);

    printf("[Parent %d] Bye !\n", getpid());
    return EXIT_SUCCESS;
}

void SIGUSR1_handler(int signum)
{
    (void)(signum); // Unused

    printf("PID %d : SIGUSR1 received !\n", getpid());
}