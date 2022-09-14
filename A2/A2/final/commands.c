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
            struct stat fileStat;
            char *file_name = list_dir[i];
            stat(file_name, &fileStat);
            // if (stat(file_name, &fileStat) < 0)
            // {
            //     perror("Error: Unable to get file stats");
            //     return;
            // }
            if ((S_ISDIR(fileStat.st_mode)))
            {
                printBlue();
                printf("%s  ", list_dir[i]);
                resetColor();
            }
            else if ((fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH))
            {
                printGreen();
                printf("%s  ", list_dir[i]);
                resetColor();
            }
            else
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
            if ((S_ISDIR(fileStat.st_mode)))
            {
                printBlue();
                printf("%s  ", list_dir[i]);
                resetColor();
            }
            else if ((fileStat.st_mode & S_IXUSR) || (fileStat.st_mode & S_IXGRP) || (fileStat.st_mode & S_IXOTH))
            {
                printGreen();
                printf("%s  ", list_dir[i]);
                resetColor();
            }
            else
                printf("%s  ", list_dir[i]);
            printf("\n");
        }
    }
    if (!op_l)
        printf("\n");

    // Close the directory
    closedir(dh);
}

// pinfo command

void _pinfo(char *pid, char *old_dir)
{
    // if no pid is given then we will print info of current process
    if (pid == NULL)
    {
        pid = (char *)malloc(100 * sizeof(char));
        sprintf(pid, "%d", getpid());
    }

    // print pid

    printf("pid : %s\n", pid);

    // print process status

    char *status_file = (char *)malloc(100 * sizeof(char));
    sprintf(status_file, "/proc/%s/status", pid);
    FILE *fp = fopen(status_file, "r");
    if (fp == NULL)
    {
        perror("Error: Unable to open status file");
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    // while ((read = getline(&line, &len, fp)) != -1)
    // {
    //     if (strstr(line, "State") != NULL)
    //     {
    //         printf("Process Status : %s", line);
    //         break;
    //     }
    // }

    // print the process status
    char *stat_file = (char *)malloc(100 * sizeof(char));
    sprintf(stat_file, "/proc/%s/stat", pid);
    FILE *fp2 = fopen(stat_file, "r");
    if (fp2 == NULL)
    {
        perror("Error: Unable to open stat file");
        return;
    }
    char *line2 = NULL;
    size_t len2 = 0;
    ssize_t read2;
    while ((read2 = getline(&line2, &len2, fp2)) != -1)
    {
        char *token = strtok(line2, " ");
        int i = 0;
        char temp[1000];
        while (token != NULL)
        {
            if (i == 2)
            {
                printf("Process Status : %s", token);
                // break;
            }
            if (i == 4)
            {
                strcpy(temp, token);
            }
            if (i == 7)
            {
                if(strcmp(temp, token) == 0)
                {
                    printf("+\n");
                    break;
                }
            }
            
            token = strtok(NULL, " ");
            i++;
        }
    }

    line2 = NULL;
    len2 = 0;
    char *pgid;

    while ((read2 = getline(&line2, &len2, fp2)) != -1)
    {
        char *token = strtok(line2, " ");
        int i = 0;
        while (token != NULL)
        {
            if (i == 4)
            {
                strcpy(pgid, token);
                break;
            }
            token = strtok(NULL, " ");
            i++;
        }
    }
    fclose(fp2);

    fclose(fp);
    // print memory

    char *mem_file = (char *)malloc(100 * sizeof(char));
    sprintf(mem_file, "/proc/%s/statm", pid);
    fp = fopen(mem_file, "r");
    if (fp == NULL)
    {
        perror("Error: Unable to open statm file");
        return;
    }
    line = NULL;
    len = 0;
    read = getline(&line, &len, fp);
    char *memory = strtok(line, " ");
    printf("memory : %s  {virtual memory}\n", memory);
    fclose(fp);

    // print executable path

    char *exe_file = (char *)malloc(100 * sizeof(char));
    sprintf(exe_file, "/proc/%s/exe", pid);
    char *exe_path = (char *)malloc(100 * sizeof(char));
    int len_exe = readlink(exe_file, exe_path, 100);
    exe_path[len_exe] = '\0';
    printf("Executable Path : ~%s\n", exe_path + strlen(old_dir));
}

// Create a custom discover command which emulates the basics of the find command. The command should search for files in a directory hierarchy.

void _discover(char *dir, char *name, char *old_dir, int op_d, int op_f)
{
    // if no directory is given then we will search in current directory
    if (dir == NULL)
    {
        dir = (char *)malloc(100 * sizeof(char));
        getcwd(dir, 100);
    }
    // if no name is given then we will search for all files
    if (name == NULL)
    {
        name = (char *)malloc(100 * sizeof(char));
        strcpy(name, "");
    }
    // open the directory
    DIR *dh = opendir(dir);
    if (dh == NULL)
    {
        perror("Error: Unable to open directory");
        return;
    }
    // read the directory
    struct dirent *d;
    // While the next entry is not readable we will print directory files
    while ((d = readdir(dh)) != NULL)
    {
        // If hidden files are found we continue
        if (d->d_name[0] == '.')
            continue;
        // if name is given then we will search for that name
        if (strcmp(name, "") != 0)
        {
            if (strstr(d->d_name, name) == NULL)
                continue;
        }
        // // if both op_f and op_d are given then we will search for both files and directories
        // if (op_f && op_d)
        // {
        //     if (d->d_type != DT_REG && d->d_type != DT_DIR)
        //         continue;
        // }

        // if op_d is given then we will search for directories
        if (op_d)
        {
            if (d->d_type != DT_DIR)
                continue;
        }
        // if op_f is given then we will search for files
        if (op_f)
        {
            if (d->d_type != DT_REG)
                continue;
        }

        // print the file name
        printf("%s/%s \n", dir, d->d_name);
    }
    // close the directory
    closedir(dh);
    // open the directory again
    dh = opendir(dir);
    // While the next entry is not readable we will search for files in subdirectories
    while ((d = readdir(dh)) != NULL)
    {
        // If hidden files are found we continue
        if (d->d_name[0] == '.')
            continue;
        // if the entry is a directory then we will search for files in that directory
        if (d->d_type == DT_DIR)
        {
            char *new_dir = (char *)malloc(100 * sizeof(char));
            sprintf(new_dir, "%s/%s", dir, d->d_name);
            _discover(new_dir, name, old_dir, op_d, op_f);
        }
    }
    // close the directory
    closedir(dh);
}

// Create a custom history command which emulates the basics of the history command. The command should print the last 10 commands executed by the shell.

void _history()
{
    // open the history file
    FILE *fp = fopen("/home/yash/college/third_year/OSN/OSN-Monsoon-2022-/A2/A2/history.txt", "r");
    if (fp == NULL)
    {
        perror("Error: Unable to open history file");
        return;
    }
    // read the history file
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    // while ((read = getline(&line, &len, fp)) != -1)
    // {
    //     // print the last 10 commands
    //     if (count >= 10)
    //         break;
    //     printf("%s", line);
    //     count++;
    // }

    // print the last 10 lines of the file

    int lines = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        lines++;
    }
    fclose(fp);
    fp = fopen("/home/yash/college/third_year/OSN/OSN-Monsoon-2022-/A2/A2/history.txt", "r");
    count = 0;
    // if the command is encountered twice consecutively then we will not print it
    char *prev_line = (char *)malloc(100 * sizeof(char));
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (count >= lines - 10)
        {
            if (strcmp(line, prev_line) != 0)
            {
                printf("%s", line);
                strcpy(prev_line, line);
            }
        }
        count++;
    }
    fclose(fp);
}
