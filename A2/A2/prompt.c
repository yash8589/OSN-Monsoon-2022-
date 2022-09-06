#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include <string.h>
#include <unistd.h>

char old_dir[1024];
int len_old_dir;

// name of the current working directory
void pwd(char w_dir[])
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcpy(w_dir, cwd);
}

// name of the current user using getlogin_r
void whoami(char name_user[])
{
    char user[1024];
    getlogin_r(user, sizeof(user));
    // printf("%s\n", user);
    strcpy(name_user, user);
}

// name of the system
void hostname(char name_host[])
{
    char host[1024];
    gethostname(host, sizeof(host));
    // printf("%s\n", host);
    strcpy(name_host, host);
}

char *init_shell()
{
    // directory from where the program is running
    pwd(old_dir);
    len_old_dir = strlen(old_dir);
    int to_clear = 1; // whether to clear the screen
    if (to_clear)
    {
        printf("\e[1;1H\e[2J");
    }
    to_clear = 0;
    return old_dir;
}

void prompt()
{

    char w_dir[1000];
    char name_user[1000];
    char name_host[1000];

    // name of the working directory
    pwd(w_dir);
    // name of the current user
    whoami(name_user);
    // name of the system
    hostname(name_host);
    // print the prompt
    if (strlen(w_dir) >= strlen(old_dir))
    // printf("%s@%s:~%s$ ", name_user, name_host, w_dir + strlen(old_dir));
    {
        printGreen();
        printf("%s@%s:", name_user, name_host);
        resetColor();
        printBlue();
        printf("~%s ", w_dir + strlen(old_dir));
        resetColor();
        
    }
    else
        printf("%s@%s:~%s$ ", name_user, name_host, w_dir);
}
