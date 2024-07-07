#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 8192

int main(void)
{
    int n;
    char buf[BUF_SIZE];

    while ((n = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buf, n) != n)
        {
            perror("write failed");
        }
    }

    if (n < 0)
    {
        perror("read err");
    }

    exit(0);
}