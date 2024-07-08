#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    write(STDOUT_FILENO, "test test\n", 10);
    write(STDERR_FILENO, "test err\n", 9);
    exit(0);
}