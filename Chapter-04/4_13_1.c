#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    
    if (truncate("test_file1", 5) < 0 )
    {
        perror("truncate fail 1");
    }
    if (truncate("test_file2", 15) < 0)
    {
        perror("truncate fail 2");
    }

    exit(0);
}