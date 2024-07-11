[TOC]

## 实验环境
```
myx@myx-virtual-machine:~/myFiles/APUE/Chapter-04$ uname -a
Linux myx-virtual-machine 5.4.0-187-generic #207-Ubuntu SMP Mon Jun 10 08:16:10 UTC 2024 x86_64 x86_64 x86_64 GNU/Linux
```

## 4.6 文件和文件夹的所有权

    我们使用open和create，包括shell中touch得到的文件属于哪个用于和组呢？

    这里先讨论文件（不包括文件夹）
    文件的user ID是当前进程的effective ID(有效用户ID)
    文件的group ID分两种情况:（1）当前进程的group ID; （2）该文件所在文件夹的group ID
    在我所用的Linux5.4.0的环境下:
    (1)如果当前所在文件夹的set-group-ID 位设置了,则文件的group ID是当前文件夹得group ID
    (2)否则是当前进程的effective ID

    这里用到一个命令,如何给文件夹设置set-group-ID位
``` chmod +s <file> ```
    分别用两个用户(myx myx2)创建两个文件夹test1 test2, 并设置set-group-ID位
```
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ stat test1 test2
  文件：test1
  大小：4096            块：8          IO 块：4096   目录
设备：801h/2049d        Inode：525606      硬链接：2
权限：(0777/drwxrwxrwx)  Uid：( 1000/     myx)   Gid：( 1000/     myx)
最近访问：2024-07-11 22:43:22.305121538 +0800
最近更改：2024-07-11 22:43:21.161140742 +0800
最近改动：2024-07-11 22:43:21.161140742 +0800
创建时间：-
  文件：test2
  大小：4096            块：8          IO 块：4096   目录
设备：801h/2049d        Inode：525607      硬链接：2
权限：(6777/drwsrwsrwx)  Uid：( 1001/    myx2)   Gid：( 1001/    myx2)
最近访问：2024-07-11 22:43:22.305121538 +0800
最近更改：2024-07-11 22:43:21.161140742 +0800
最近改动：2024-07-11 22:43:21.161140742 +0800
创建时间：-
```
然后分别在两个文件夹中用用户(myx)创建两个文件,可以看到两个文件的group ID的情况
```
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ touch test1/file1 test2/file2
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ stat test1/file1 test2/file2
  文件：test1/file1
  大小：0               块：0          IO 块：4096   普通空文件
设备：801h/2049d        Inode：524507      硬链接：1
权限：(0664/-rw-rw-r--)  Uid：( 1000/     myx)   Gid：( 1000/     myx)
最近访问：2024-07-11 22:50:46.505661397 +0800
最近更改：2024-07-11 22:50:46.505661397 +0800
最近改动：2024-07-11 22:50:46.505661397 +0800
创建时间：-
  文件：test2/file2
  大小：0               块：0          IO 块：4096   普通空文件
设备：801h/2049d        Inode：525236      硬链接：1
权限：(0664/-rw-rw-r--)  Uid：( 1000/     myx)   Gid：( 1001/    myx2)
最近访问：2024-07-11 22:50:46.505661397 +0800
最近更改：2024-07-11 22:50:46.505661397 +0800
最近改动：2024-07-11 22:50:46.505661397 +0800
创建时间：-
```
## 4.7 *access* 和 *faccessat* 函数 

先看api
```
#include <unistd.h>

int access(const char *pathname, int mode);

#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>

int faccessat(int dirfd, const char *pathname, int mode, int flags);
```

*mode*
    F_OK    测试文件是否存在
    R_OK    测试是否有权限读文件(同时也会测试文件是否存在)
    W_OK    测试是否有权限写文件(同时也会测试文件是否存在)
    X_OK    测试是否有权限执行文件(同时也会测试文件是否存在)

对于*access*是检查real user ID和real group ID是否有权限

对于*faccessat*当 <1> *pathname*是绝对路径 或者 <2> fd是AT_FDCWD, *pathname*是相对路径 时, 和access效果一样
默认(flags设置为0???)情况下, *faccessat* 检查real user ID和real group ID的权限
当flags设置为AT_EACCESS时, 检查effective user ID和effective group ID的权限

4_7_1.c
```c
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
```
```
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ ll /etc/passwd file_not_exist
ls: 无法访问 'file_not_exist': 没有那个文件或目录
-rw-r--r-- 1 root root 3111 7月  11 22:40 /etc/passwd

myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ ./4_7_1.out /etc/passwd
file exist test ok
file read access test ok
file write access test err: Permission denied
file execute access test err: Permission denied
open for write fail: Permission denied

myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ ./4_7_1.out file_not_exist
file not exist: No such file or directory
file read access err: No such file or directory
file write access test err: No such file or directory
file execute access test err: No such file or directory
open for write fail: No such file or directory

myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ su
密码： 
root@myx-virtual-machine:/home/myx/APUE/APUE/Chapter-04# chown root:root 4_7_1.out 
root@myx-virtual-machine:/home/myx/APUE/APUE/Chapter-04# chmod u+s 4_7_1.out 
root@myx-virtual-machine:/home/myx/APUE/APUE/Chapter-04# ll 4_7_1.out 
-rwsrwxr-x 1 root root 16912 7月  12 00:10 4_7_1.out*
root@myx-virtual-machine:/home/myx/APUE/APUE/Chapter-04# exit
exit
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ ./4_7_1.out /etc/passwd
file exist test ok
file read access test ok
file write access test err: Permission denied
file execute access test err: Permission denied
open for write success
myx@myx-virtual-machine:~/APUE/APUE/Chapter-04$ 
```
这个例子最后, 进程的real user ID时普通用户(myx), effective user ID是root, 所以我们可以用读方式打开/etc/passwd(此时检查的是effective user ID的权限), 
但是faccessat测试real user ID(myx)却没权限写这个文件

**这里有个注意点:书上那个例子里的/etc/passwd的权限和我们环境的可能不一样, 所以运行效果和书上的也不一样**