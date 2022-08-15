/*
--------------------NOTES--------------------

OPEN syscall: long long open (const char* Path, long long flags [, long long mode ]);
CLOSE syscall: long long close (long long filedes);
LSEEK syscall: off_t lseek(long long fd, off_t offset, long long whence);
READ syscall: ssize_t read(long long fd, void *buf, size_t count);
WRITE syscall: ssize_t write(long long fd, const void *buf, size_t count);

--------------------NOTES--------------------
*/

/*
--------------------QUESTION--------------------

- Check the permissions of the file
- Check whether the content in the new file are the reverse of the old file

PATH of input: ./a.out < newf ile >< oldf ile >< directory >

--------------------QUESTION--------------------
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat sb;
    // check if the file address has been provided
    if (argc < 3)
    {
        write(2, "ERROR!!!\n", 10);
        exit(1);
    }
    // check if direcory exists

    if (stat(argv[3], &sb) == -1)
    {
        printf("Directory: %s", argv[3]);
        write(STDOUT_FILENO, "Assignment directory does not exist\n", strlen("Assignment directory does not exist\n"));
        perror("Error");
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }
    return 0;
}