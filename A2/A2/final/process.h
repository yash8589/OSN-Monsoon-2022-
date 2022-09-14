#ifndef __PROCESS_H
#define __PROCESS_H

void _execute(char **args, char *old_dir);
// int _background(char **args, char *old_dir);
int _background(char *input, char *old_dir);

#endif
