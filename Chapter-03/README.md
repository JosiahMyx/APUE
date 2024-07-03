3_6_1.c
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    if (-1 == lseek(STDIN_FILENO, 0, SEEK_CUR))
    {
        perror("can not seek");
    }
    else
    {
        printf("seek ok\n");
    }

    exit(0);
}
```
```
myx@myx-virtual-machine:~/myFiles/APUE/Chapter-03$ ./3_6_1.out < /etc/passwd
seek ok
myx@myx-virtual-machine:~/myFiles/APUE/Chapter-03$ cat /etc/passwd | ./3_6_1.out 
can not seek: Illegal seek
```