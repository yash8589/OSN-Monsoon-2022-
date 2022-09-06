#ifndef __COMMANDS_H
#define __COMMANDS_H

void _ls(const char *dir, int op_a, int op_l);
void _pinfo(char *pid, char *old_dir);
void _discover(char *dir, char *name, char *old_dir, int op_d, int op_f);
void _history();

#endif
