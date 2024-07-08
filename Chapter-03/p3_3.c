#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show(int fd1, int fd2, int fd3)
{
    int fd1_fdf, fd2_fdf, fd3_fdf;
    int fd1_fsf, fd2_fsf, fd3_fsf;

    if ((fd1_fdf = fcntl(fd1, F_GETFD)) < 0)
    {
        perror("getfd fail 1");
        exit(-1);
    }
    printf("fd1_fdf:%d ", fd1_fdf);
    if ((fd1_fsf = fcntl(fd1, F_GETFL)) < 0)
    {
        perror("getfl fail 1");
        exit(-1);
    }
    printf("fd1_fsf:%d\n", fd1_fsf);
    if ((fd2_fdf = fcntl(fd2, F_GETFD)) < 0)
    {
        perror("getfd fail 2");
        exit(-1);
    }
    printf("fd2_fdf:%d ", fd2_fdf);
    if ((fd2_fsf = fcntl(fd2, F_GETFL)) < 0)
    {
        perror("getfl fail 2");
        exit(-1);
    }
    printf("fd2_fsf:%d\n", fd2_fsf);
    if ((fd3_fdf = fcntl(fd3, F_GETFD)) < 0)
    {
        perror("getfd fail 3");
        exit(-1);
    }
    printf("fd3_fdf:%d ", fd3_fdf);
    if ((fd3_fsf = fcntl(fd3, F_GETFL)) < 0)
    {
        perror("getfl fail 3");
        exit(-1);
    }
    printf("fd3_fsf:%d\n", fd3_fsf);
}

int main(void)
{
    int fd1, fd2, fd3;


    if ((fd1 = open("test.file", O_RDWR)) < 0)
    {
        perror("open fail");
        exit(-1);
    }

    if ((fd2 = dup(fd1)) < 0)
    {
        perror("dup fail");
        exit(-1);
    }

    if ((fd3 = open("test.file", O_RDONLY)) < 0)
    {
        perror("open fail 1");
        exit(-1);
    }

    /* printf("fd1:%d fd2:%d fd3:%d\n", fd1, fd2, fd3); */
    printf("original\n");
    show(fd1, fd2, fd3);

    if ((fcntl(fd1, F_SETFD, fcntl(fd1, F_GETFD) | FD_CLOEXEC)) < 0)
    {
        perror("fcntl err 1");
        exit(-1);
    }
    printf("after modify fd1 fdf\n");
    show(fd1, fd2, fd3);

    if ((fcntl(fd1, F_SETFL, fcntl(fd1, F_GETFL) | O_APPEND)) < 0)
    {
        perror("fcntl err 2");
        exit(-2);
    }
    printf("after modify fd1 fsf\n");
    show(fd1, fd2, fd3);

    exit(0);
}