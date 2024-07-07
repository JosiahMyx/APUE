#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

char g_buf_1[] = "abcdefghij";
char g_buf_2[] = "ABCDEFGHIJ";

int main(void)
{
    int i_fd;

    if ((i_fd = open("file.hole", O_RDWR | O_CREAT | O_TRUNC, 0744)) < 0)
    {
        perror("open file.hole failed");
        goto fail1;
    }

    if (10 != write(i_fd, g_buf_1, 10))
    {
        perror("write 1 failed");
        goto fail2;
    }

    if (-1 == lseek(i_fd, 16384, SEEK_SET))
    {
        perror("lseek failed");
        goto fail2;
    }

    if (10 != write(i_fd, g_buf_2, 10))
    {
        perror("write 2 failed");
        goto fail2;
    }

    close(i_fd);
    exit(0);

fail2:
    close(i_fd);
fail1:
    exit(-1);
}