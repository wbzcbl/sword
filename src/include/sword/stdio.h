#ifndef SWORD_STDIO_H
#define SWORD_STDIO_H

#include <sword/stdarg.h>

int vsprintf(char* buf, const char* fmt, va_list args);
int sprintf(char* buf, const char* fmt, ...);

#endif