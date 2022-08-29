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
        prompt();
        char *input = get_input();
        char *input_no_spaces = removeSpacesFromStr(input);
        parse(input_no_spaces);
    }
}
