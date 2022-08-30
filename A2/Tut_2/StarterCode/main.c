#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include <string.h>
#include <unistd.h>

// init_shell() initializes the shell
// prompt() prints the prompt
// get_input() takes input from the user

// TODO: '&' operator run the command preceding it in the background after printing the process id of the newly created process.
// TODO: Specification 2: Builtin commands [20 marks] --> [cd - 10 marks]

int main()
{
    init_shell();
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

                command = strtok(NULL, ";\n");
            }
            max_args = j;
            j = 0;
            if (strcmp(sep_commands[j], "cd") == 0)
            {
                if (j + 1 < max_args)
                {
                    if (chdir(sep_commands[j + 1]) != 0)
                    {
                        perror("Error: No such directory");
                    }
                }
                else
                {
                    perror("Error: No directory specified");
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
            else
            {
                perror("Error: No such command");
            }
        }
    }
}
