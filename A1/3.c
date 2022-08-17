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
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct stat buf;
    if (stat(argv[3], &buf) == -1) // check if Directory exists
    {
        perror("ERROR!!!\n");
    }
    else
    {
        if (S_ISDIR(buf.st_mode))
        {
            write(STDOUT_FILENO, "Directory is created: Yes\n", strlen("Directory is created: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Directory is created: No\n", strlen("Directory is created: No\n"));
        }

        struct stat file_1;
        char *file_name1 = argv[2]; // new file

        if (argc != 4)
        {
            write(2, "ERROR!!!\n", 10);
            exit(1);
        }

        char *dest1 = argv[1]; // new file

        if (stat(dest1, &file_1) == -1)
        {
            perror("Error");
        }
        else
        {
            if (file_1.st_mode & S_IRUSR)
            {
                write(STDOUT_FILENO, "User has read permission on new file: Yes\n", strlen("User has read permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has read permission on new file: No\n", strlen("User has read permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IWUSR)
            {
                write(STDOUT_FILENO, "User has write permission on new file: Yes\n", strlen("User has write permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has write permission on new file: No\n", strlen("User has write permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IXUSR)
            {
                write(STDOUT_FILENO, "User has execute permission on new file: Yes\n", strlen("User has execute permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has execute permission on new file: No\n", strlen("User has execute permission on new file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));

            // Group permissions

            if (file_1.st_mode & S_IRGRP)
            {
                write(STDOUT_FILENO, "Group has read permission on new file: Yes\n", strlen("Group has read permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has read permission on new file: No\n", strlen("Group has read permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IWGRP)
            {
                write(STDOUT_FILENO, "Group has write permission on new file: Yes\n", strlen("Group has write permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has write permission on new file: No\n", strlen("Group has write permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IXGRP)
            {
                write(STDOUT_FILENO, "Group has execute permission on new file: Yes\n", strlen("Group has execute permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has execute permission on new file: No\n", strlen("Group has execute permission on new file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));

            // Others permissions

            if (file_1.st_mode & S_IROTH)
            {
                write(STDOUT_FILENO, "Others has read permission on new file: Yes\n", strlen("Others has read permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has read permission on new file: No\n", strlen("Others has read permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IWOTH)
            {
                write(STDOUT_FILENO, "Others has write permission on new file: Yes\n", strlen("Others has write permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has write permission on new file: No\n", strlen("Others has write permission on new file: No\n"));
            }

            if (file_1.st_mode & S_IXOTH)
            {
                write(STDOUT_FILENO, "Others has execute permission on new file: Yes\n", strlen("Others has execute permission on new file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has execute permission on new file: No\n", strlen("Others has execute permission on new file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));
        }

        struct stat file_2;        // old file
        char *filename2 = argv[2]; // old file
        char *dest2 = malloc(sizeof(char) * 500);
        strcat(dest2, filename2);

        if (stat(dest2, &file_2) == -1)
        {
            perror("Error\n");
        }
        else
        {
            if (file_2.st_mode & S_IRUSR)
            {
                write(STDOUT_FILENO, "User has read permission on old file: Yes\n", strlen("User has read permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has read permission on old file: No\n", strlen("User has read permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IWUSR)
            {
                write(STDOUT_FILENO, "User has write permission on old file: Yes\n", strlen("User has write permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has write permission on old file: No\n", strlen("User has write permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IXUSR)
            {
                write(STDOUT_FILENO, "User has execute permission on old file: Yes\n", strlen("User has execute permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "User has execute permission on old file: No\n", strlen("User has execute permission on old file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));

            // Group permissions

            if (file_2.st_mode & S_IRGRP)
            {
                write(STDOUT_FILENO, "Group has read permission on old file: Yes\n", strlen("Group has read permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has read permission on old file: No\n", strlen("Group has read permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IWGRP)
            {
                write(STDOUT_FILENO, "Group has write permission on old file: Yes\n", strlen("Group has write permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has write permission on old file: No\n", strlen("Group has write permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IXGRP)
            {
                write(STDOUT_FILENO, "Group has execute permission on old file: Yes\n", strlen("Group has execute permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Group has execute permission on old file: No\n", strlen("Group has execute permission on old file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));

            // Others permissions

            if (file_2.st_mode & S_IROTH)
            {
                write(STDOUT_FILENO, "Others has read permission on old file: Yes\n", strlen("Others has read permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has read permission on old file: No\n", strlen("Others has read permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IWOTH)
            {
                write(STDOUT_FILENO, "Others has write permission on old file: Yes\n", strlen("Others has write permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has write permission on old file: No\n", strlen("Others has write permission on old file: No\n"));
            }

            if (file_2.st_mode & S_IXOTH)
            {
                write(STDOUT_FILENO, "Others has execute permission on old file: Yes\n", strlen("Others has execute permission on old file: Yes\n"));
            }
            else
            {
                write(STDOUT_FILENO, "Others has execute permission on old file: No\n", strlen("Others has execute permission on old file: No\n"));
            }
            write(STDOUT_FILENO, "\n", strlen("\n"));
        }

        if (buf.st_mode & S_IRUSR)
        {
            write(STDOUT_FILENO, "User has read permission on directory: Yes\n", strlen("User has read permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "User has read permission on directory: No\n", strlen("User has read permission on directory: No\n"));
        }

        if (buf.st_mode & S_IWUSR)
        {
            write(STDOUT_FILENO, "User has write permission on directory: Yes\n", strlen("User has write permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "User has write permission on directory: No\n", strlen("User has write permission on directory: No\n"));
        }

        if (buf.st_mode & S_IXUSR)
        {
            write(STDOUT_FILENO, "User has execute permission on directory: Yes\n", strlen("User has execute permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "User has execute permission on directory: No\n", strlen("User has execute permission on directory: No\n"));
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));

        // Group permissions

        if (buf.st_mode & S_IRGRP)
        {
            write(STDOUT_FILENO, "Group has read permission on directory: Yes\n", strlen("Group has read permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Group has read permission on directory: No\n", strlen("Group has read permission on directory: No\n"));
        }

        if (buf.st_mode & S_IWGRP)
        {
            write(STDOUT_FILENO, "Group has write permission on directory: Yes\n", strlen("Group has write permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Group has write permission on directory: No\n", strlen("Group has write permission on directory: No\n"));
        }

        if (buf.st_mode & S_IXGRP)
        {
            write(STDOUT_FILENO, "Group has execute permission on directory: Yes\n", strlen("Group has execute permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Group has execute permission on directory: No\n", strlen("Group has execute permission on directory: No\n"));
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));

        // Others permissions

        if (buf.st_mode & S_IROTH)
        {
            write(STDOUT_FILENO, "Others has read permission on directory: Yes\n", strlen("Others has read permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Others has read permission on directory: No\n", strlen("Others has read permission on directory: No\n"));
        }

        if (buf.st_mode & S_IWOTH)
        {
            write(STDOUT_FILENO, "Others has write permission on directory: Yes\n", strlen("Others has write permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Others has write permission on directory: No\n", strlen("Others has write permission on directory: No\n"));
        }

        if (buf.st_mode & S_IXOTH)
        {
            write(STDOUT_FILENO, "Others has execute permission on directory: Yes\n", strlen("Others has execute permission on directory: Yes\n"));
        }
        else
        {
            write(STDOUT_FILENO, "Others has execute permission on directory: No\n", strlen("Others has execute permission on directory: No\n"));
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }

    // check if the content of the new file is the reverse of the old file
    long long new = open(argv[1], O_RDONLY);
    long long old = open(argv[2], O_RDONLY);
    // printf("%d\n", new);

    off_t fileLength_new = lseek(new, 0, SEEK_END);
    // printf("%d\n", fileLength_new);
    off_t fileLength_old = lseek(old, 0, SEEK_END);
    // printf("%d\n", fileLength_old);

    if (fileLength_new != fileLength_old)
    {
        write(STDOUT_FILENO, "Whether file contents are reversed in newfile: No\n", strlen("Whether file contents are reversed in newfile: No\n"));
        return 0;
    }
    else
    {

        lseek(new, 0, SEEK_SET);
        lseek(old, -1, SEEK_END);

        // creation of chunks to optimize the working of the code
        long long chunksize;
        long long byte_size = fileLength_new / 500;
        if (byte_size < 1)
        {
            chunksize = 1;
        }
        else
        {
            chunksize = (long long)pow(2, (long long)(log((double)(fileLength_new * (0.002))) / log(2.0)));
        }
        // printf("%d\n", chunksize);

        // c is the chunk of bytes to be read, and r is the reverse of c
        char *c = (char *)malloc(sizeof(char) * chunksize);
        char *r = (char *)malloc(sizeof(char) * chunksize);

        // index of the last chunk of the input file
        long long start = ((fileLength_new - 1) / chunksize) * chunksize;
        // length of the last chunk of the input file
        long long len_to_move = fileLength_new - start;
        // cursor at the end of the input file
        lseek(old, start, SEEK_SET);
        while (1)
        {
            read(old, c, len_to_move);
            read(new, r, len_to_move);
            // printf("%s\n", c);
            // reversing the chunk of bytes
            for (long long i = 0; i < len_to_move; i++)
            {
                if (r[i] != c[len_to_move - i - 1])
                {
                    write(STDOUT_FILENO, "Whether file contents are reversed in newfile: No\n", strlen("Whether file contents are reversed in newfile: No\n"));
                    return 0;
                }
            }
            r[len_to_move] = ' ';
            if (lseek(old, 0, SEEK_CUR) - chunksize - len_to_move < 0)
            {
                break;
            }
            lseek(old, -chunksize - len_to_move, SEEK_CUR);
            len_to_move = chunksize;
        }
        write(STDOUT_FILENO, "Whether file contents are reversed in newfile: Yes\n", strlen("Whether file contents are reversed in newfile: Yes\n"));
    }
    return 0;
}