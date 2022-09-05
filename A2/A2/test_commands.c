
void ls()
{
    char *argv[1000];
    char *input = get_input();
    long long argc = 0;
    argc = parse(input, argc, argv);
    if (argc == 1)
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                printf("%s ", dir->d_name);
            }
            printf("Happened\n");
            closedir(d);
        }
    }
}