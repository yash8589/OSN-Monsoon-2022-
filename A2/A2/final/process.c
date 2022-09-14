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
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>

// execute sys commands in foreground
void _execute(char **args, char *old_dir)
{

    int status;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        // child process
        if (execvp(args[0], args) < 0)
        {
            perror("Error");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        // error forking
        perror("Error");
        exit(1);
    }
    else
    {
        // parent process
        waitpid(pid, &status, WUNTRACED);
    }
}

// execute sys commands in background

// int _background(char **args, char *old_dir)
// {

//     int status;
//     pid_t pid;
//     pid = fork();
//     if (pid == 0)
//     {
//         // child process
//         if (execvp(args[0], args) < 0)
//         {
//             perror("Error");
//             exit(1);
//         }
//     }
//     else if (pid < 0)
//     {
//         // error forking
//         perror("Error");
//         exit(1);
//     }
//     else
//     {
//         // parent process
//         // waitpid(pid, &status, WUNTRACED);

//         // print the pid of the newly created process
//     }
//     printf("PID: %d \n", pid);
//     return pid;
// }

int _background(char *input, char *old_dir)
{
    pid_t pid = fork();
    char* input_copy;
    strcpy(input_copy, input);
    int num_cum = 0;
    if (pid == 0)
    {
        // tokenise the input
        char *token = strtok(input, " &\n\t\r");
        num_cum++;
        while (token != NULL)
        {
            token = strtok(NULL, " &\n\t\r");
            num_cum++;
        }
        char *args[num_cum + 1];
        token = strtok(input_copy, " &\n\t\r");
        int i = 0;
        while (token != NULL)
        {
            args[i] = token;
            token = strtok(NULL, " &\n\t\r");
            i++;
        }
        args[i] = NULL;
        // strcpy(old_dir, args[0]);
        if (execvp(args[0], args) < 0)
        {
            perror("Error");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        // error forking
        perror("Error");
        exit(1);
    }
    
    return pid;
}