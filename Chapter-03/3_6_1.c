#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    if (-1 == lseek(STDIN_FILENO, 0, SEEK_CUR))
    {
        printf("can not seek\n");
    }
    else
    {
        printf("seek ok\n");
    }

    exit(0);
}