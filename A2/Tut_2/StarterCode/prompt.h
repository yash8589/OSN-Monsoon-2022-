#ifndef __PROMPT_H
#define __PROMPT_H

void init_shell();
void prompt();
void pwd(char w_dir[]);
void whoami(char name_user[]);
void hostname(char name_host[]);

#endif
