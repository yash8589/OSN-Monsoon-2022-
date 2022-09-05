#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include "commands.h"
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

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
    for (int i = 0; i < num_dir; i++)
    {
        printf("%s  ", list_dir[i]);
    }
    printf("\n");

    if (!op_l)
        printf("\n");

    // Close the directory
    closedir(dh);
}
