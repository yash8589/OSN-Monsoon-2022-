#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include "commands.h"
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

// ls commands with flags implemented without using execvp and execv

// ls()

// TODO: demoss

void _ls(const char *dir, int op_a, int op_l)
{
    char *list_dir[10000];
    // Here we will list the directory
    struct dirent *d;
    DIR *dh = opendir(dir);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            // If the directory is not found
            perror("Directory doesn't exist");
            return;
        }
        else
        {
            // If the directory is not readable then throw error and exit
            perror("Unable to read directory");
            return;
        }
        // exit(EXIT_FAILURE);
    }
    // While the next entry is not readable we will print directory files
    int num_dir = 0;
    while ((d = readdir(dh)) != NULL)
    {
        // If hidden files are found we continue
        if (!op_a && d->d_name[0] == '.')
            continue;
        // printf("%s  ", d->d_name);
        list_dir[num_dir] = d->d_name;
        num_dir++;
        // if (op_l)
        //     printf("\n");
    }
    // sort list_dir in alphabetical order
    for (int i = 0; i < num_dir; i++)
    {
        for (int j = i + 1; j < num_dir; j++)
        {
            if (strcmp(list_dir[i], list_dir[j]) > 0)
            {
                char *temp = list_dir[i];
                list_dir[i] = list_dir[j];
                list_dir[j] = temp;
            }
        }
    }
    // print list_dir
    if (!op_l)
    {
        for (int i = 0; i < num_dir; i++)
        {
            printf("%s  ", list_dir[i]);
        }
    }
    else
    {
        // print in long listing format
        for (int i = 0; i < num_dir; i++)
        {
            struct stat fileStat;
            char *file_name = list_dir[i];
            if (stat(file_name, &fileStat) < 0)
            {
                perror("Error: Unable to get file stats");
                return;
            }
            // print file permissions
            printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
            printf(" ");
            // print number of hard links
            printf("%ld ", fileStat.st_nlink);
            // print user name
            struct passwd *pw = getpwuid(fileStat.st_uid);
            printf("%s ", pw->pw_name);
            // print group name
            struct group *gr = getgrgid(fileStat.st_gid);
            printf("%s ", gr->gr_name);
            // print file size
            printf("%ld ", fileStat.st_size);
            // print last modified time
            char *time = ctime(&fileStat.st_mtime);
            time[strlen(time) - 1] = '\0';
            printf("%s ", time);
            // print file name
            printf("%s ", file_name);
            printf("\n");
        }
        printf("\n");
    }
    if (!op_l)
        printf("\n");

    // Close the directory
    closedir(dh);
}
