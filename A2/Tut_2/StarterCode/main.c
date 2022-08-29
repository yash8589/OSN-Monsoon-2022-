#include "prompt.h"
#include "headers.h"

int main()
{
    init_shell();
    while (1)
    {
        prompt();
        char a[20];
        scanf("%s", a);
        // TAKE INPUT HERE
    }
}
