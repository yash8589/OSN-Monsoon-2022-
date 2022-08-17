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

Reverse the file except the part that that is specified at the start of the execution of the code as the start and the end index of the character not to be reversed.

--------------------QUESTION--------------------
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    // check if the file address has been provided
    if (argc != 4)
    {
        write(2, "ERROR!!!\n", 10);
        exit(1);
    }
    // Reading both the Input and Output files using syscalls
    long long source_file = open(argv[1], O_RDONLY);
    // printf("%d\n", source_file);
    // printf("%s\n", argv[1]);

    // fileLength is the length of the file in bytes
    off_t fileLength = lseek(source_file, 0, SEEK_END);
    // printf("%ld\n", fileLength);

    // create the destination folder
    mkdir("Assignment", 0700);

    // creating the destination file
    char destination[100] = "Assignment/2_";
    strcat(destination, argv[1]);

    long long dest_file = open(destination, O_CREAT | O_RDWR | O_TRUNC, 0600);

    // Error handling
    if (source_file < 0)
    {
        write(2, "Error in file operations", 25);
        exit(1);
    }
    if (fileLength < 1)
    {
        write(2, "Error in file operations", 25);
        exit(1);
    }
    if (dest_file < 0)
    {
        write(2, "Error in file operations", 25);
        exit(1);
    }

    long long begin = atoi(argv[2]);
    long long end = atoi(argv[3]);

    // cursor at the beginning of the output file
    lseek(dest_file, 0, SEEK_SET);
    // cursor at the end of the input file
    lseek(source_file, -1, SEEK_END);

    // creation of chunks to optimize the working of the code
    long long chunksize;
    long long byte_size = fileLength / 500;
    if (byte_size < 1)
    {
        chunksize = 1;
    }
    else
    {
        chunksize = (long long)pow(2, (long long)(log((double)(fileLength * (0.002))) / log(2.0)));
    }

    // printf("%d\n", chunksize);

    // c is the chunk of bytes to be read, and r is the reverse of c
    char *c = (char *)malloc(sizeof(char) * chunksize);
    char *r = (char *)malloc(sizeof(char) * chunksize);

    // index of the last chunk of the input file
    long long start = ((fileLength - 1) / chunksize) * chunksize;
    // length of the last chunk of the input file
    long long len_to_move = fileLength - start;
    // cursor at the end of the input file
    lseek(source_file, start, SEEK_SET);

    long long total_progress = 0;
    char buffer[100];
    char message[100];

    // printf("\nFileLength: %ld \nNo of bytes per read: %d \nStart: %d \nChunk Size: %d\n\n", fileLength, chunksize, start, len_to_move);
    // main loop
    while (1)
    {
        // printf("\nLocation: %lld\tLength read: %d\t", lseek(source_file, 0, SEEK_CUR), len_to_move);
        // reading the chunk of bytes

        // TODO: make a functioning progressbar
        ssize_t read_size = read(source_file, c, len_to_move);
        total_progress += read_size;
        // sprintf(message, "\r Reversing the entire file\n");
        // write(STDOUT_FILENO, message, strlen(message));
        // fflush(stdout);
        sprintf(buffer, "\r progress = %.2f%%", (100.0f * total_progress) / fileLength);
        write(STDOUT_FILENO, buffer, strlen(buffer));
        fflush(stdout);

        // printf("%s\n", c);
        // reversing the chunk of bytes
        for (long long i = 0; i < len_to_move; i++)
        {
            r[i] = c[len_to_move - i - 1];
        }
        r[len_to_move] = ' ';
        // printf("%s\n", r);
        // writing the reversed chunk of bytes to the output file
        write(dest_file, r, len_to_move);
        // printf("%s\n", r);
        if (lseek(source_file, 0, SEEK_CUR) - chunksize - len_to_move < 0)
        {
            break;
        }
        lseek(source_file, -chunksize - len_to_move, SEEK_CUR);
        len_to_move = chunksize;
    }

    // lseek(dest_file, 0, SEEK_SET);
    // // cursor at the end of the input file
    // lseek(source_file, -1, SEEK_END);

    // creation of chunks to optimize the working of the code
    byte_size = end - begin;
    chunksize = byte_size;

    // printf("%d\n", chunksize);

    // c is the chunk of bytes to be read, and r is the reverse of c
    char *c1 = (char *)malloc(sizeof(char) * chunksize);
    char *r1 = (char *)malloc(sizeof(char) * chunksize);

    // index of the last chunk of the input file
    start = fileLength - begin;
    // length of the last chunk of the input file
    len_to_move = byte_size;
    // cursor at the end of the input file
    lseek(dest_file, fileLength - end, SEEK_SET);

    total_progress = 0;
    char buffer1[100];
    char message1[100];

    // printf("\nFileLength: %ld \nNo of bytes per read: %d \nStart: %d \nChunk Size: %d\n\n", fileLength, chunksize, start, len_to_move);
    // main loop
    while (1)
    {
        // printf("\nLocation: %lld\tLength read: %d\t", lseek(source_file, 0, SEEK_CUR), len_to_move);
        // reading the chunk of bytes

        // TODO: make a functioning progressbar
        ssize_t read_size = read(dest_file, c1, len_to_move);
        total_progress += read_size;
        // sprintf(message1, "\r Reversing the middle section\n");
        // write(STDOUT_FILENO, message1, strlen(message1));
        // fflush(stdout);
        sprintf(buffer1, "\r progress = %.2f%%", (100.0f * total_progress) / (end - begin));
        write(STDOUT_FILENO, buffer1, strlen(buffer1));
        fflush(stdout);

        // printf("%s\n", c);
        // reversing the chunk of bytes
        for (long long i = 0; i < len_to_move; i++)
        {
            r1[i] = c1[len_to_move - i - 1];
        }
        r1[len_to_move] = ' ';
        // printf("%s\n", r);
        // writing the reversed chunk of bytes to the output file
        lseek(dest_file, fileLength - end, SEEK_SET);
        write(dest_file, r1, len_to_move);
        // printf("%s\n", r1);
        if (lseek(source_file, 0, SEEK_CUR) - chunksize - len_to_move < 0)
        {
            break;
        }
        lseek(source_file, -chunksize - len_to_move, SEEK_CUR);
        len_to_move = chunksize;
    }
    close(source_file);
    close(dest_file);
    return 0;
}