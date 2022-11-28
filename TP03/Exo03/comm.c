
#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE] = {'\0'};
int pipes[2] = {0};

void SIGUSR1_handler(int signum)
{
    (void)(signum); // Unused

    time_t current_time = time(NULL);

    sprintf(buffer, "%s \tPID %d : SIGUSR1 received !\n", ctime(&current_time), getpid());
    write(pipes[1], buffer, strlen(buffer) + 1);
}
void SIGUSR2_handler(int signum)
{
    (void)(signum); // Unused

    time_t current_time = time(NULL);

    sprintf(buffer, "%s \tPID %d : SIGUSR2 received !\n", ctime(&current_time), getpid());
    write(pipes[1], buffer, strlen(buffer) + 1);
}

int main(void)
{
    pid_t pid;
    pid_t children_pids[2] = {0};

    if(pipe(pipes) == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

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
            close(pipes[0]);

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

    close(pipes[1]);
    printf("Parent PID %d sending signals...\n", getpid());

    sleep(2);

    for(int i = 0 ; i < 3 ; ++i)
    {
        kill(children_pids[0], SIGUSR1);
        read(pipes[0], buffer, BUFFER_SIZE);
        printf("Received pipe data : %s", buffer);
        // Print date
        sleep(1);

        kill(children_pids[1], SIGUSR2);
        read(pipes[0], buffer, BUFFER_SIZE);
        printf("Received pipe data : %s", buffer);
        // Print date
        sleep(1);
    }

    sleep(3);

    kill(children_pids[0], SIGKILL);
    kill(children_pids[1], SIGKILL);

    return EXIT_SUCCESS;
}
