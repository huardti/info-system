#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "I need a string and a list of one or more files !\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;

    for(int i = 2 ; i < argc ; ++i)
    {
        pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0)
        {
            printf("Grep PID %d lancé, en attente...\n", pid);
            waitpid(pid, NULL, 0);
            printf("Grep PID %d terminé.\n", pid);
        }
        else
        {
            execlp("grep", "grep", argv[1], argv[i], NULL);
            break;
        }
    }

    return EXIT_SUCCESS;
}