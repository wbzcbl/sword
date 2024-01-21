#include <sword/console.h>
#include <sword/stdarg.h>
#include <sword/stdio.h>

static char buf[1024];

int printk(const char* fmt, ...){
    va_list args;
    int i;

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    console_write(buf, i);

    return i;
}