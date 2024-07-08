#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd;
    off_t lseek_offset;
    char buf[1024] = {0};
    int read_len;

    if ((fd = open("testfile", O_RDWR | O_APPEND)) < 0)
    {
        perror("open failed 1");
        exit(-1);
    }

    if((lseek_offset = lseek(fd, 4, SEEK_SET)) != 4)
    {
        perror("lseek err 1");
        exit(-1);
    }

    if ((read_len = read(fd, buf, 1)) < 0)
    {
        perror("read fail");
        exit(-1);
    }

    if (write(STDOUT_FILENO, buf, 1) != 1)
    {
        perror("write fail 1");
        exit(-1);
    }
    putchar('\n');

    if((lseek_offset = lseek(fd, 5, SEEK_SET)) != 5)
    {
        perror("lseek err 2");
        exit(-1);
    }

    if ((write(fd, "x", 1)) != 1)
    {
        perror("write fail 2");
        exit(-1);
    }

    exit(0);
}