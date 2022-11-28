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
void SIGINT_handler(int signum)
{
    (void)(signum); // Unused

    printf("PID %d : SIGINT received !\n", getpid());
}

int main(void)
{
    signal(SIGUSR1, SIGUSR1_handler);
    signal(SIGUSR2, SIGUSR2_handler);
    signal(SIGINT, SIGINT_handler);

    printf("PID %d, waiting...\n", getpid());
    for(;;);

    return EXIT_SUCCESS;
}
