#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include <string.h>
#include <unistd.h>

#define MAX_COMMANDS 100

char *removeSpacesFromStr(char *string)
{
    // non_space_count to keep the frequency of non space characters
    int non_space_count = 0;

    // Traverse a string and if it is non space character then, place it at index non_space_count
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ' ')
        {
            string[non_space_count] = string[i];
            non_space_count++; // non_space_count incremented
        }
    }

    // Finally placing final character at the string end
    string[non_space_count] = '\0';
    return string;
}

char *removeSpacesFromCommand(char *string)
{
    // non_space_count to keep the frequency of non space characters
    int non_space_count = 0;

    // Traverse a string and if it is non space character then, place it at index non_space_count
    for (int i = 0; string[i] != '\0'; i++)
    {
        if ((string[i] == ' ' && string[i + 1] == ' ') != 1)
        {
            string[non_space_count] = string[i];
            non_space_count++; // non_space_count incremented
        }
    }

    // Finally placing final character at the string end
    string[non_space_count] = '\0';
    return string;
}

// parse the code and return the number of commands
long long parse(char *input, long long argc, char *argv[1000])
{
    char command_list[MAX_COMMANDS][1000];
    // remove spaces from the input
    char *input_no_spaces = removeSpacesFromCommand(input);

    // count the number of commands
    int num_commands = 0;
    for (int i = 0; input_no_spaces[i] != '\0'; i++)
    {
        if (input_no_spaces[i] == ';' || input_no_spaces[i] == '&')
        {
            num_commands++;
        }
    }

    // separate the commands
    char *command = strtok(input_no_spaces, ";\n");
    while (command != NULL)
    {
        // printf("%s\n", command);
        argv[argc] = command;
        argc++;

        command = strtok(NULL, ";&\n");
    }

    return argc;
}