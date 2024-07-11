#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        exit(-1);
    }

    if (faccessat(AT_FDCWD, argv[1], F_OK, 0) < 0)
    {
        perror("file not exist");
    }
    else
    {
        printf("file exist test ok\n");
    }

    if (faccessat(AT_FDCWD, argv[1], R_OK, 0) < 0)
    {
        perror("file read access err");
    }
    else
    {
        printf("file read access test ok\n");
    }

    if (faccessat(AT_FDCWD, argv[1], W_OK, 0) < 0)
    {
        perror("file write access test err");
    }
    else
    {
        printf("file write access test ok\n");
    }

    if (faccessat(AT_FDCWD, argv[1], X_OK, 0) < 0)
    {
        perror("file execute access test err");
    }
    else
    {
        printf("file execute access test err\n");
    }

    if (open(argv[1], O_WRONLY) < 0)
    {
        perror("open for write fail");
    }
    else
    {
        printf("open for write success\n");
    }

    exit(0);
}