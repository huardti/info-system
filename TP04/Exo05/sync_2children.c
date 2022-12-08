#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "sem.h"

#define TRUE 1
#define FALSE 0

int main(void)
{
    printf("[Parent] PID %d\n", getpid());

    SEMAPHORE sem[] = {
        sem_create(FALSE, "/tmp/lol", 0),
        sem_create(FALSE, "/tmp/lol", 1)
    };

    for(int i = 0 ; i < 2 ; ++i)
    {
        sem_init(sem[i], (unsigned short[]){1});
    }

    pid_t children_pids[2] = {0};

    for(int i = 0 ; i < 2 ; ++i)
    {
        pid_t pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            return EXIT_FAILURE;
        }
        else if(pid > 0) // Parent
        {
            printf("[Parent] Created child PID %d\n", pid);
            children_pids[i] = pid;
        }
        else // Child
        {
            printf("[Child] PID %d created by parent PID %d\n", getpid(), getppid());

            for(;;)
            {
                printf("[Child PID %d] Waiting...\n", getpid());
                P(sem[i]);

                printf("[Child PID %d] NEXT !\n", getpid());
            }

            // Should never be reached
            printf("[Child] PID %d Exiting", getpid());
            return EXIT_SUCCESS;
        }
    }

    int current_child = 0;
    for(int i = 0 ; i < 10 ; ++i)
    {
        printf("[Parent] Unlocking child PID %d\n", children_pids[current_child]);
        V(sem[current_child]);

        current_child ^= 1;
        sleep(1);
    }

    for(int i = 0 ; i < 2 ; ++i)
    {
        printf("[Parent] Killing child PID %d\n", children_pids[i]);
        kill(children_pids[i], SIGKILL);
    }
    puts("[Parent] Exiting");
}