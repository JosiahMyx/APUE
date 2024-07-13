#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    struct stat stStatBuf;

    /* modify the mode */
    if (stat("foo", &stStatBuf) < 0)
    {
        perror("stat fail");
        exit(-1);
    }

    if (chmod("foo", stStatBuf.st_mode | ~(S_IXGRP) | S_ISGID) < 0)
    {
        perror("chmod 1 err");
        exit(-1);
    }

    /* set the absolute mode */
    if (chmod("bar", S_IRUSR | S_IRGRP | S_IROTH) < 0)
    {
        perror("chmod 2 err");
        exit(-1);
    }

    exit(0);
}