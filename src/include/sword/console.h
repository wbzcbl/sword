#ifndef SWORD_CONSOLE_H
#define SWORD_CONSOLE_H

#include<sword/types.h>

void console_init();
void console_clear();
void console_write(char *buf, u32 count);

#endif