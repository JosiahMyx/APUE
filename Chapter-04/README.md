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
## 4.7 access 和 faccessat 函数 

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

**这里有个注意点:书上那个例子里的/etc/passwd的权限和我们环境的可能不一样, 所以运行效果和书上的也不一样(例如我的环境other是有读权限的)**

## 4.8 umask 函数

```c
#include <sys/stat.h>

mode_t umask(mode_t cmask);
返回旧的umask的值
```
创建的文件权限是 mode & (~umask)
就是umask哪一位设置了,那这位表示的权限默认关闭

4_8_1.c
```c
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
```

```
myx@myx-virtual-machine:~/APUE/Chapter-04$ umask
0002
myx@myx-virtual-machine:~/APUE/Chapter-04$ ./4_8_1.out 
myx@myx-virtual-machine:~/APUE/Chapter-04$ ll foo bar
-rw-r--r-- 1 myx myx 0 7月  12 23:02 bar
-rw-rw-rw- 1 myx myx 0 7月  12 23:02 foo
myx@myx-virtual-machine:~/APUE/Chapter-04$ umask
0002
```
可以看到:
(1) umask设置group write 和 other write位, bar 的权限两位就被清除了
(2) 执行这个程序前后, shell umask的值无变化, 一个进程修改自己的umask无法影响父进程的umask

其中umask返回的0002的意思这么理解, 第一个0表示这是8进制数
其他三位部分是二进制的三个bit
第一 0  二进制000   依次表示user的 读/写/执行 权限
第二 0  二进制000   依次表示group的 读/写/执行 权限
第三 2  二进制010   依次表示other的 读/写/执行 权限

umask命令可以显示当前shell中的umask的值
umask -S 可以显示符号
```
myx@myx-virtual-machine:~/APUE/Chapter-04$ umask
0002
myx@myx-virtual-machine:~/APUE/Chapter-04$ umask -S
u=rwx,g=rwx,o=rx
```
不过-S显示的是umask允许的权限位, 相反的是, umask置位的是禁止的权限位

## 4.9 chmod fchmod fchmodat 函数

```
#include <sys.stat.h>
int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
int fchmodat(int fd, const char *pathname, mode_t mode, int flag);
返回值: ok 0, -1表示出错
```
这三个函数可以修改已经存在的文件的访问权限:
(1) chmod修改特定文件的访问权限
(2) fchmod修改打开的文件的访问权限
(3) fchmodat当 <1> pathname是绝对路径 或 <2> fd是AT_FDCWD且pathname是相对路径 时, 和chmod一样
当flag设置了AT_SYMLINK_NOFOLLOW, fchmodat不会follow 符号链接

**要修改文件的权限, 进程的effective ID必须和文件的拥有者ID一致, 或者进程拥有root权限**

参数中的mode可以是以下常量 **与** 在一起
| mode | 描述 |
| :--- | :--- |
|S_ISUID| set-user-ID|
|S_ISGID| set-group-ID|
|S_ISVTX| saved-text(sticky bit)|
|S_IRWXU| user(owner) r w x|
|S_IRUSR| user r|
|S_IWUSR| user w|
|S_IXUSR| user x|
|S_IRWXG| grp r w x|
|S_IRGRP| grp r|
|S_IWGRP| grp w|
|S_IXGRP| grp x|
|S_IRWXO| other r w x|
|S_IROTH| other r|
|S_IWOTH| other w|
|S_IXOTH| other x|

4_9_1.c
```c
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
```
```
myx@myx-virtual-machine:~/APUE/Chapter-04$ ll foo bar
-rw-r--r-- 1 myx myx 0 7月  12 23:02 bar
-rw-rw-rw- 1 myx myx 0 7月  12 23:02 foo
myx@myx-virtual-machine:~/APUE/Chapter-04$ ./4_9_1.out 
myx@myx-virtual-machine:~/APUE/Chapter-04$ ll foo bar
-r--r--r-- 1 myx myx 0 7月  12 23:02 bar
-rwsrwSrwt 1 myx myx 0 7月  12 23:02 foo*
```
对bar的修改和预想的一致
对foo的修改, S表示设置了set-group-ID但是没有执行权限, **TODO 这里为什么自动设置了set-user-ID位和粘粘位？还没搞懂**

```
myx@myx-virtual-machine:~/APUE/Chapter-04$ stat bar
  文件：bar
  大小：0               块：0          IO 块：4096   普通空文件
设备：801h/2049d        Inode：525414      硬链接：1
权限：(0644/-rw-r--r--)  Uid：( 1000/     myx)   Gid：( 1000/     myx)
最近访问：2024-07-12 23:02:05.669108028 +0800
最近更改：2024-07-12 23:02:05.669108028 +0800
最近改动：2024-07-13 21:06:07.895522745 +0800
创建时间：-
myx@myx-virtual-machine:~/APUE/Chapter-04$ ./4_9_1.out 
myx@myx-virtual-machine:~/APUE/Chapter-04$ stat bar
  文件：bar
  大小：0               块：0          IO 块：4096   普通空文件
设备：801h/2049d        Inode：525414      硬链接：1
权限：(0444/-r--r--r--)  Uid：( 1000/     myx)   Gid：( 1000/     myx)
最近访问：2024-07-12 23:02:05.669108028 +0800
最近更改：2024-07-12 23:02:05.669108028 +0800
最近改动：2024-07-13 21:06:23.971026721 +0800
创建时间：-
myx@myx-virtual-machine:~/APUE/Chapter-04$
```
同时可以看到, 修改权限仅 **最近改动(last change time)** 这里的时间更新了, **最近更改(last modify time)** 这里的时间没更新
而且上面ll那看到的时间也没变化, 是因为last change time反应的是inode修改的时间, last modify time反应的是文件内容修改的时间
ll默认显示的时间是last modify time

 chmod对2个bit的自动处理情况:
（1）Linux中对普通文件设置sticky bit无意义
（2）创建一个新文件，如果进程的组ID和附属组ID都不等于文件的组ID, 并且进程也没有root权限, 那么set-group-ID位默认关闭


## 4.10 sticky bit

以前是给文件设置这个标记, 那么这个文件被执行了, 会被拷贝到swap, 方便下次调用, 但是这个功能已经过时了, linux中给普通文件设置这个位没什么作用
现在的作用是给文件夹设置这个位，那么如果想要删除或者重命名这个文件夹中的文件，需要同时满足:
(1) 对这个文件夹有写权限
(2) 是这个文件的所有者或者文件夹的所有者或者是root用户
一个例子就是/tmp
```
myx@myx-virtual-machine:~/APUE/Chapter-04$ ll -d /tmp
drwxrwxrwt 22 root root 4096 7月  14 00:08 /tmp/
myx@myx-virtual-machine:/tmp$ su myx2
密码： 
myx2@myx-virtual-machine:/tmp$ touch test_for_sticky
myx2@myx-virtual-machine:/tmp$ su myx
密码： 
myx@myx-virtual-machine:/tmp$ 
myx@myx-virtual-machine:/tmp$ ll test_for_sticky 
-rw-rw-r-- 1 myx2 myx2 0 7月  14 00:09 test_for_sticky
myx@myx-virtual-machine:/tmp$ rm test_for_sticky 
rm：是否删除有写保护的普通空文件 'test_for_sticky'？ y
rm: 无法删除 'test_for_sticky': 不允许的操作
myx@myx-virtual-machine:/tmp$ mv test_for_sticky test_for_sticky1
mv: 无法将'test_for_sticky' 移动至'test_for_sticky1': 不允许的操作
```
**简单记一下Linux中这些权限位的作用**

|标志|文件|文件夹|
|---|---|---|
|读|读文件内容 |查看文件夹中的文件列标 |
|写|修改文件内容 |在文件夹中创建文件、删除文件、重命名文件 |
|执行|执行二进制文件 |进入目录 |
|set-user-ID|执行文件的时候进程有效ID是文件的拥有者ID |无意义 |
|set-group-ID|无意义 |文件夹中创建的文件(包括文件夹)的组ID继承文件夹的组ID |
|sticky bit|无意义 |对文件夹有写权限且(是文件的拥有者或是文件夹的拥有者或是root用户)才能删除或重命名文件 |

## 4.11 chown fchown fchownat lchown

