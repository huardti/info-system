#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

long unsigned int get_inode(char* path)
{
    FILE* fd = fopen(path, "r");

    if(!fd)
    {
        fprintf(stderr, "Unable to open file %s\n", path);
        exit(EXIT_FAILURE);
    }

    struct stat fileinfo = {0};
    fstat(fileno(fd), &fileinfo);

    fclose(fd);

    return fileinfo.st_ino;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "I need a filename !\n");
        exit(EXIT_FAILURE);
    }

    printf("Inode number of %s is %ld\n", argv[1], get_inode(argv[1]));

    return EXIT_SUCCESS;
}