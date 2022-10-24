#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXFILS 2

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "I need a string and a list of one or more files !\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    int fils = 0;

    for(int i = 2 ; i < argc ; ++i)
    {
        if(fils >= MAXFILS)
        {
            printf("Attente : PID %d terminé.\n", wait(NULL));
            --fils;
        }

        pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0)
        {
            printf("Grep PID %d lancé, en attente...\n", pid);
            ++fils;
        }
        else
        {
            execlp("grep", "grep", argv[1], argv[i], NULL);
            break;
        }
    }

    for(int i = 0 ; i < MAXFILS ; ++i)
    {
        printf("Grep PID %d terminé.\n", wait(NULL));
    }

    return EXIT_SUCCESS;
}