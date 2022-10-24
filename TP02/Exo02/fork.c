#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int nb_enfants = 7;
    pid_t pid = -1;

    int i;
    for(i = 1 ; i <= nb_enfants ; ++i)
    {
        pid = fork();

        if(pid < 0)
        {
            perror("fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid > 0)
        {
            printf
            (
                "Je suis le parent %d de l'enfant %d, il gambade...\n"
                , getpid()
                , pid
            );

            waitpid(pid, NULL, 0);

            printf("Ça y est l'enfant %d s'est cassé la gueule.\n", pid);
        }
        else
        {
            printf
            (
                "%d : HAHAHAH BLBLBLBL JE SUIS %d HIHIHIHI BLBLBLBL\n"
                , i
                , getpid()
            );
            break;
        }
    }

    if(pid == 0)
    {
        printf("%d : *se vautre* AAAAAAAAH\n", i);
    }

    return EXIT_SUCCESS;
}