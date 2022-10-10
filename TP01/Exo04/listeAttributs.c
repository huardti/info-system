#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "I need a filename !\n");
        exit(EXIT_FAILURE);
    }

    FILE* fd = fopen(argv[1], "r");

    if(!fd)
    {
        fprintf(stderr, "Unable to open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    struct stat fileinfo = {0};
    fstat(fileno(fd), &fileinfo);

    printf("Inode number of %s is %ld\n", argv[1], fileinfo.st_ino);

    return EXIT_SUCCESS;
}