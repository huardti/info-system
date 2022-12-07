#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if(pid > 0) // Parent
    {
        printf("Parent PID %d created child PID %d\n", getpid(), pid);
    }
    else // Child
    {
        printf("Child PID %d created by parent PID %d\n", getpid(), getppid());
    }

    return EXIT_SUCCESS;
}