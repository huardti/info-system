#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 100

int main(void)
{
    int pipes[2];
    if(pipe(pipes) == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0) // Parent
    {
        close(pipes[0]);
        sleep(2);

        char buffer[BUFFER_SIZE] = {'\0'};
        sprintf
        (
            buffer
            , "The parent has PID %d and the child has PID %d"
            , getpid()
            , pid
        );
        write(pipes[1], buffer, strlen(buffer)+1);
        close(pipes[1]);

        printf("Sent `%s` in pipe\n", buffer);
    }
    else // Child
    {
        close(pipes[1]);
        char buffer[BUFFER_SIZE] = {'\0'};

        read(pipes[0], buffer, sizeof(buffer)/sizeof(buffer[0]));
        printf("Received pipe data : `%s`\n", buffer);
        close(pipes[0]);
    }

    return EXIT_SUCCESS;
}