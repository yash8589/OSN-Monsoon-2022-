#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include "commands.h"
#include "process.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

// TODO: Spec: 4 ,spec 6, spec 7, spec 8 (whole)

// init_shell() initializes the shell
// prompt() prints the prompt
// get_input() takes input from the user

int main()
{
    char *old_dir = init_shell();
    // printf("%s", old_dir);
    char current_dir[1024];
    // strcpy(current_dir, old_dir);
    pwd(current_dir);
    char prev_dir[1024];
    pwd(prev_dir);

    char buffer[100];

    int history_count = 0; // history count of 1 session
    int child_pid = 0;
    int status;

    while (1)
    {

        int max_args = 0;
        long long argc = 0; // number of arguments
        char *argv[1000];   // array of arguments
        prompt();
        char *input = get_input();
        char *input_copy = (char *)malloc(1000 * sizeof(char));
        strcpy(input_copy, input);
        // char *input_no_spaces = removeSpacesFromStr(input);
        // long long temp = parse(input_no_spaces, argc, argv);
        long long temp = parse(input, argc, argv);
        argc = temp; // number of arguments

        // printf("%lld\n", argc);
        // for (int i = 0; i < argc; i++)
        // {
        //     printf("%s\n", argv[i]);
        // }
        char name_child_process[10000];
        for (int i = 0; i < argc; i++)
        {
            // fgets(buffer, sizeof(buffer), argv[i]);
            // append argv[i] to history file
            FILE *history_file = fopen("/home/yash/college/third_year/OSN/OSN-Monsoon-2022-/A2/A2/history.txt", "a+");
            // add a new line to the history file

            fprintf(history_file, "%s", argv[i]);
            fprintf(history_file, "%s", "\n");
            // fprintf(history_file, "%s", " ");
            history_count++;
            fclose(history_file);

            // printf("%s\n", argv[i]);

            int j = 0;
            char *sep_commands[1000];

            char *command = strtok(argv[i], " ");
            while (command != NULL)
            {
                sep_commands[j] = command;
                j++;
                // printf("%s\n", command);

                command = strtok(NULL, " ");
            }
            max_args = j;
            j = 0;

            if (strcmp(sep_commands[j], "cd") == 0)
            {
                // fprintf(history_file, "%s", argv[i]);

                if (j + 1 < max_args)
                {
                    long len_cur_dir = strlen(current_dir);

                    if (strcmp(sep_commands[j + 1], "-") == 0)
                    {
                        chdir(prev_dir);
                    }
                    else if (strcmp(sep_commands[j + 1], "~") == 0)
                    {
                        chdir(old_dir);
                    }
                    else if (chdir(sep_commands[j + 1]) != 0)
                    {
                        perror("Error: No such directory");
                    }
                    // get the current directory in history for cd -
                    pwd(current_dir);

                    strcpy(prev_dir, current_dir);
                    prev_dir[len_cur_dir] = '\0';
                    // printf("%s\n", current_dir);
                    // printf("%s\n", prev_dir);
                    // printf("%ld\n", strlen(sep_commands[j + 1]));
                }
                else
                {
                    // perror("Error: No directory specified");
                    chdir(old_dir);
                }
            }
            else if (strcmp(sep_commands[j], "pwd") == 0)
            {
                // fprintf(history_file, "%s", argv[i]);
                char w_dir[1000];
                pwd(w_dir);
                printf("%s\n", w_dir);
            }
            else if (strcmp(sep_commands[j], "echo") == 0)
            {
                // fprintf(history_file, "%s", argv[i]);
                if (j + 1 < max_args)
                {
                    printf("%s\n", sep_commands[j + 1]);
                }
                else
                {
                    perror("Error: No string specified");
                }
            }
            // implement ls, ls -l, ls -a without using execvp

            // TODO:  ls <filename>
            else if (strcmp(sep_commands[j], "ls") == 0)
            {
                // fprintf(history_file, "%s", argv[i]);
                int flag_a = 0;
                int flag_l = 0;
                int num_file_names = 0; // number of file names
                char *filenames[1000];

                if (max_args == 1)
                {
                    _ls(".", 0, 0);
                }
                else
                {
                    while (j + 1 < max_args)
                    {

                        if (strcmp(sep_commands[j + 1], "-a") == 0)
                        {
                            flag_a = 1;
                        }
                        else if (strcmp(sep_commands[j + 1], "-l") == 0)
                        {
                            flag_l = 1;
                        }
                        else if (strcmp(sep_commands[j + 1], "-al") == 0 || strcmp(sep_commands[j + 1], "-la") == 0)
                        {
                            flag_a = 1;
                            flag_l = 1;
                        }
                        else
                        {
                            filenames[num_file_names] = sep_commands[j + 1];
                            num_file_names++;
                        }
                        j++;
                    }
                    if (num_file_names > 0)
                    {
                        for (int i = 0; i < num_file_names; i++)
                        {
                            if (strcmp(filenames[i], "~") == 0)
                            {
                                _ls(old_dir, flag_a, flag_l);
                            }
                            else
                            {
                                _ls(filenames[i], flag_a, flag_l);
                            }
                        }
                    }
                    else
                    {
                        _ls(".", flag_a, flag_l);
                    }
                }
            }

            // implement the pinfo command

            // TODO: +/- for foreground processes and background processes
            else if (strcmp(sep_commands[j], "pinfo") == 0)
            {
                char *lastchar = argv[i] + strlen(argv[i]) - 1;
                // fprintf(history_file, "%s", argv[i]);
                if (j + 1 < max_args)
                {
                    _pinfo(sep_commands[j + 1], old_dir);
                }
                else
                {
                    char *pid = (char *)malloc(1000 * sizeof(char));
                    sprintf(pid, "%d", getpid());
                    _pinfo(pid, old_dir);
                }
            }

            // Create a custom discover command which emulates the basics of the find command. The command should search for files in a directory hierarchy.

            // TODO: discover -d -f

            else if (strcmp(sep_commands[j], "discover") == 0)
            {
                // fprintf(history_file, "%s", argv[i]);
                int flag_d = 0;
                int flag_f = 0;
                char *dir_name = NULL;
                char *file_name = NULL;
                int file_hier = 0; // number of file names
                char *file_hierchy[1000];

                while (j + 1 < max_args)
                {
                    if (strcmp(sep_commands[j + 1], "-d") == 0)
                    {
                        flag_d = 1;
                    }
                    else if (strcmp(sep_commands[j + 1], "-f") == 0)
                    {
                        flag_f = 1;
                    }
                    else
                    {
                        file_hierchy[file_hier] = sep_commands[j + 1];
                        file_hier++;
                    }

                    j++;
                }
                if (flag_d && flag_f)
                {
                    flag_d = 0;
                    flag_f = 0;
                }
                // printf("%d   %d", flag_d, flag_f);
                if (file_hier == 2)
                {
                    dir_name = file_hierchy[0];
                    file_name = file_hierchy[1];
                }
                else if (file_hier == 1)
                {
                    dir_name = file_hierchy[0];
                }
                else
                {
                    dir_name = ".";
                }
                if (strcmp(dir_name, "~") == 0)
                {
                    dir_name = old_dir;
                }
                _discover(dir_name, file_name, old_dir, flag_d, flag_f);
            }

            // history command
            else if (strcmp(sep_commands[j], "history") == 0)
            {
                _history();
            }
            else
            {
                char *lastchar = input_copy + strlen(input_copy) - 2;
                if (strcmp(lastchar, "&\n") == 0)
                {
                    // // // fprintf(history_file, "%s", argv[i]);
                    // int z = 0;
                    // char **new_command = (char **)malloc(1000 * sizeof(char));
                    // // printf("Hello2\n");
                    // // remove last 2 characters from input_copy
                    // input_copy[strlen(input_copy) - 2] = '\0';
                    // // printf("Hello\n");
                    // strcpy(new_command[z], input_copy);
                    // // printf("World\n");
                    char temp[10000];
                    strcpy(temp, input_copy);
                    char *token = strtok(temp, " &\n\t\r");
                    strcpy(name_child_process, token);
                    child_pid = _background(input_copy, old_dir);
                    printCyan();
                    printf("PID: %d\n", child_pid);
                    resetColor();
                    // z++;

                    // display a message to the user that the process running in the background has exited
                }
                else
                {
                    time_t start, end;
                    time(&start);
                    _execute(sep_commands, old_dir);
                    time(&end);
                    if (difftime(end, start) > 1)
                    {
                        printYellow();
                        printf("(took %.0f seconds) --> ", difftime(end, start));
                        resetColor();
                    }
                    // printf("Time taken: %d seconds\n", time_taken);
                }
                // perror("Error: No such command");
            }
        }
        if (child_pid == 0)
        {
            continue;
        }
        else
        {
            if (waitpid(child_pid, &status, WNOHANG) > 0)
            {
                if (WEXITSTATUS(status))
                {
                    printCyan();
                    printf("%s process with PID %d exited abnormally\n", name_child_process, child_pid);
                    resetColor();
                }
                else
                {
                    printGreen();
                    printf("%s process with PID %d exited normally\n", name_child_process, child_pid);
                    resetColor();
                }
                // printf("Process with pid %d exited\n", child_pid);
                child_pid = 0;
            }
            else
            {
                continue;
            }
        }
    }
}
