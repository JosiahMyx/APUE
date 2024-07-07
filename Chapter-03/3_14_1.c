#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int val;

    if (argc != 2)
    {
        printf("Usage: ./3_14_1.out <file>\n");
        return -1;
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
    {
        perror("fcntl failed");
        return -1;
    }

    switch (val & O_ACCMODE)
    {
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            printf("unkown");
            break;
    }

    if (val & O_APPEND)
    {
        printf(", APPEND");
    }

    if (val & O_NONBLOCK)
    {
        printf(", NONBLOCK");
    }

    if (val & O_SYNC)
    {
        printf(", SYNC");
    }

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_SYNC != O_FSYNC)
    if (val & O_FSYNC)
    {
        printf(", FSYNC");
    }
#endif

    putchar('\n');
    exit(0);
}