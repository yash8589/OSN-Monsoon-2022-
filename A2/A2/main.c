#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include "commands.h"
#include <string.h>
#include <unistd.h>

// init_shell() initializes the shell
// prompt() prints the prompt
// get_input() takes input from the user

// TODO: '&' operator run the command preceding it in the background after printing the process id of the newly created process.

int main()
{
    char *old_dir = init_shell();
    // printf("%s", old_dir);
    char current_dir[1024];
    // strcpy(current_dir, old_dir);
    pwd(current_dir);
    char prev_dir[1024];
    pwd(prev_dir);
    while (1)
    {

        int max_args = 0;
        long long argc = 0; // number of arguments
        char *argv[1000];   // array of arguments
        prompt();
        char *input = get_input();
        // char *input_no_spaces = removeSpacesFromStr(input);
        // long long temp = parse(input_no_spaces, argc, argv);
        long long temp = parse(input, argc, argv);
        argc = temp; // number of arguments

        // printf("%lld\n", argc);
        // for (int i = 0; i < argc; i++)
        // {
        //     printf("%s\n", argv[i]);
        // }

        for (int i = 0; i < argc; i++)
        {
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
                char w_dir[1000];
                pwd(w_dir);
                printf("%s\n", w_dir);
            }
            else if (strcmp(sep_commands[j], "echo") == 0)
            {
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

            // TODO: ls -l, ls <filename>
            else if (strcmp(sep_commands[j], "ls") == 0)
            {
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
            else
            {
                perror("Error: No such command");
            }
        }
    }
}
