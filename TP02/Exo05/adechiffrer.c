#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid_p = -1;
    int i = 2;
    int j = 2;

    while (i > 0 && pid_p != 0)
    {
        pid_p = fork();
        i--;
    }
    while (j > 0 && pid_p == 0)
    {
        pid_p = fork();
        j--;
    }
    if (pid_p != 0)
    {
        printf("Bonjour\n");
    }
    else
    {
        printf("A+\n");
    };

    return 0;
}