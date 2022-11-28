#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 100

int main(void)
{
    if(mkfifo("fifo", 0666) == -1)
    {
        perror("mkfifo()");
        exit(EXIT_FAILURE);
    }

    int fd = open("fifo", O_RDWR);

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0) // Parent
    {
        sleep(2);

        char buffer[BUFFER_SIZE] = {'\0'};
        sprintf
        (
            buffer
            , "The parent has PID %d and the child has PID %d"
            , getpid()
            , pid
        );
        write(fd, buffer, strlen(buffer)+1);

        printf("Sent `%s` in FIFO\n", buffer);
    }
    else // Child
    {
        char buffer[BUFFER_SIZE] = {'\0'};

        read(fd, buffer, sizeof(buffer)/sizeof(buffer[0]));
        printf("Received FIFO data : `%s`\n", buffer);
    }

    close(fd);
    return EXIT_SUCCESS;
}