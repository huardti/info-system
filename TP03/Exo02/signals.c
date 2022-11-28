#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void SIGUSR1_handler(int signum)
{
    (void)(signum); // Unused

    printf("PID %d : SIGUSR1 received !\n", getpid());
}
void SIGUSR2_handler(int signum)
{
    (void)(signum); // Unused

    printf("PID %d : SIGUSR2 received !\n", getpid());
}

int main(void)
{
    pid_t pid;
    pid_t children_pids[2] = {0};

    for(int i = 0 ; i < 2 ; ++i)
    {
        pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0) // Parent
        {
            children_pids[i] = pid;
        }
        else// Child
        {
            if(i == 0)
            {
                for(;;)
                {
                    signal(SIGUSR1, SIGUSR1_handler);
                    pause();
                }
            }
            else
            {
                for(;;)
                {
                    signal(SIGUSR2, SIGUSR2_handler);
                    pause();
                }
            }
        }
    }

    sleep(1);
    printf("Parent PID %d sending signals...\n", getpid());

    int current_child = 0;
    for(int i = 0 ; i < 10 ; ++i)
    {
        if(current_child == 0)
        {
            kill(children_pids[current_child], SIGUSR1);
            current_child = 1;
        }
        else
        {
            kill(children_pids[current_child], SIGUSR2);
            current_child = 0;
        }

        sleep(1);
    }

    return EXIT_SUCCESS;
}
