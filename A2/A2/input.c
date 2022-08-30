#include "prompt.h"
#include "headers.h"
#include "parse.h"
#include "input.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char *get_input()
{
    // take input using getline
    // length of input is input_len
    // input is a string
    size_t len = 32;
    char *input = (char *)malloc(len * sizeof(char));
    int input_len;
    input_len = getline(&input, &len, stdin);
    // printf("%s", input);
    return input;
}