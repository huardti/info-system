#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

long unsigned int get_inode(char* path)
{
    FILE* fd = fopen(path, "r");

    if(!fd)
    {
        fprintf(stderr, "Unable to open file %s\n", path);
        return 0;
    }

    struct stat fileinfo = {0};
    fstat(fileno(fd), &fileinfo);

    fclose(fd);

    return fileinfo.st_ino;
}
int print_inode(char* path)
{
    long unsigned inode = get_inode(path);
    if(inode == 0) return -1;

    printf("Inode number of %s is %ld\n",path, get_inode(path));
    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "I need a filename !\n");
        exit(EXIT_FAILURE);
    }

    char* path = argv[1];


    FILE* fd = fopen(path, "w+");
    fputs("blblbl", fd);
    print_inode(path);

    fclose(fd);
    fd = fopen(path, "r");
    char* line = NULL;
    while(getline(&line, NULL, fd) != -1)
    {
        printf("%s", line);
    }
    print_inode(path);

    fclose(fd);
    print_inode(path);

    printf("Press Enter :");
    getchar();
    print_inode(path);

    fd = fopen(path, "a");
    print_inode(path);

    unlink(path);
    print_inode(path);

    fputs("blublu", fd);
    print_inode(path);

    printf("Press Enter :");
    getchar();
    print_inode(path);

    fclose(fd);
    print_inode(path);


    return EXIT_SUCCESS;
}