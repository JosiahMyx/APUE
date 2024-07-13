#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void)
{
    umask(0);

    if (open("foo", O_CREAT, RWRWRW) < 0)
    {
        perror("open fail 1");
        exit(-1);
    }

    umask(S_IWGRP | S_IWOTH);

    if (open("bar", O_CREAT, RWRWRW) < 0)
    {
        perror("open fail 2");
        exit(-1);
    }

    exit(0);
}