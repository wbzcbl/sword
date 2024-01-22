#include <sword/debug.h>
#include <sword/stdarg.h>
#include <sword/stdio.h>
#include <sword/printk.h>
// #include <sword/device.h>

static char buf[1024];

void debugk(char *file, int line, const char *fmt, ...)
{
    // device_t *device = device_find(DEV_SERIAL, 0);
    // if (!device)
    // {
    //     device = device_find(DEV_CONSOLE, 0);
    // }

    // int i = sprintf(buf, "[%s] [%d] ", file, line);
    // device_write(device->dev, buf, i, 0, 0);

    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    printk("[%s] [%d] %s", file, line, buf);

    // device_write(device->dev, buf, i, 0, 0);
}