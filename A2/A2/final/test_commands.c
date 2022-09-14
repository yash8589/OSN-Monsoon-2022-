#include <stdio.h>

void printGreen()
{
    printf("%s", "\033[1m\033[32m");
}

void resetColor()
{
    printf("%s", "\033[1m\033[0m");
}

int main()
{
    // char * c = "hello";
    // printf("\033[31;1;4mHello\033[0m");
    // printf("%s", "\033[1m\033[32m");
    // printf(“\033[0;31m”);
    // printf(“Hello\n”);
    // printf(“\033[0m”);
    // return 0;

    printGreen();
    printf("Hello World\n");
    resetColor();
}

void _discover(char *name, char *dir, int op_d, int op_f)
{
    // if no directory is given then we will search in current directory
    if (dir == NULL)
    {
        dir = (char *)malloc(100 * sizeof(char));
        getcwd(dir, 100);
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
    int num_dir = 0;
    while ((d = readdir(dh)) != NULL)
    {
        // If hidden files are found we continue
        if (d->d_name[0] == '.')
            continue;
        // if file name matches then we will print it
        if (strcmp(d->d_name, name) == 0)
        {
            // if -d flag is given then we will print only directories
            if (op_d)
            {
                struct stat fileStat;
                char *file_name = d->d_name;
                if (stat(file_name, &fileStat) < 0)
                {
                    perror("Error: Unable to get file stats");
                    return;
                }
                if (S_ISDIR(fileStat.st_mode))
                    printf("%s  ", d->d_name);
            }
            // if -f flag is given then we will print only files
            else if (op_f)
            {
                struct stat fileStat;
                char *file_name = d->d_name;
                if (stat(file_name, &fileStat) < 0)
                {
                    perror("Error: Unable to get file stats");
                    return;
                }
                if (!S_ISDIR(fileStat.st_mode))
                    printf("%s  ", d->d_name);
            }
            // if no flag is given then we will print both files and directories
            else
            {
                printf("%s  ", d->d_name);
            }
        }
    }
    printf("\n");
    // Close the directory
    closedir(dh);
}



// discover command to find all files or directories with given name in a directory using flags -d to look only for directories and -f to look only for files. If no flag is given then it will find both files and directories. If no directory or file name is given then it will print all the files in current directory with relative path.

void _discover(char *dir, char *name, int op_d, int op_f)
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
        printf("%s \n", d->d_name);
    }
    printf("\n");
    // close the directory
    closedir(dh);
}