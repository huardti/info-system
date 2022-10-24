#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
// int main(void)
{
    /* Un seul argument :
    if(argc != 2)
    {
        fprintf(stderr, "I need a single file path !\n");
        exit(EXIT_FAILURE);
    }

    execlp("ls", "ls", "-al", argv[1], NULL);
    */

    /* Arguments multiples : */
    if(argc < 2)
    {
        fprintf(stderr, "I need one or more file paths !\n");
        exit(EXIT_FAILURE);
    }
    char* args[argc + 2];
    args[0] = "ls";
    args[1] = "-al";

    int i;
    for(i = 0 ; i < argc - 1 ; ++i)
    {
        args[i + 2] = argv[i + 1];
    }
    args[i + 2] = NULL;

    execvp(args[0], args);

    return EXIT_SUCCESS;
}