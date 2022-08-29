#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include <string.h>
#include <unistd.h>

// init_shell() initializes the shell
// prompt() prints the prompt
// get_input() takes input from the user

int main()
{
    init_shell();
    while (1)
    {
        long long argc = 0; // number of arguments
        char *argv[1000];   // array of arguments
        prompt();
        char *input = get_input();
        char *input_no_spaces = removeSpacesFromStr(input);
        long long temp = parse(input_no_spaces, argc, argv);
        argc = temp; // number of arguments

        
        // printf("%lld\n", argc);
        // for (int i = 0; i < argc; i++)
        // {
        //     printf("%s\n", argv[i]);
        // }
    }
}
