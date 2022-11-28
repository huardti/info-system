#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void usage(const char* executable)
{
    printf("Usage : %s <PID> <Signal ID>\n\n", executable);
    printf("Signal IDs : 0 = SIGUSR1  1 = SIGUSR2  2 = SIGINT  3 = SIGKILL\n\n");

    exit(EXIT_FAILURE);
}

int main(int argc, const char* argv[])
{
    if(argc != 3) usage(argv[0]);

    int signal_id = argv[2][0] - '0';
    if(signal_id < 0 || signal_id > 3) usage(argv[0]);

    pid_t pid = atoi(argv[1]);

    switch(signal_id)
    {
        case 0:
            kill(pid, SIGUSR1);
            break;
        case 1:
            kill(pid, SIGUSR2);
            break;
        case 2:
            kill(pid, SIGINT);
            break;
        case 3:
            kill(pid, SIGKILL);
            exit(EXIT_SUCCESS);
            break;
    }

    return EXIT_SUCCESS;
}
