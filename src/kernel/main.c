#include <sword/sword.h>
#include <sword/types.h>
#include <sword/io.h>
#include<sword/string.h>

// #define CRT_ADDR_REG 0X3d4 //   CRT地址寄存器
// #define CRT_DATA_REG 0X3d5  //  CRT数据寄存器

// #define CRT_CURSOR_H 0Xe // 光标位置 高位
// #define CRT_CURSOR_L 0Xf // 光标位置 低位
char message[] = "Hello Sword!!!";
char buf[1024];

void kernel_init(){
    // outb(CRT_ADDR_REG, CRT_CURSOR_H);
    // u16 pos = inb(CRT_DATA_REG) << 8;
    // outb(CRT_ADDR_REG, CRT_CURSOR_L);
    // pos = inb(CRT_DATA_REG);

    // outb(CRT_ADDR_REG, CRT_CURSOR_H);
    // outb(CRT_DATA_REG, 0);
    // outb(CRT_ADDR_REG, CRT_CURSOR_L);
    // outb(CRT_DATA_REG, 200);

    int res;
    res = strcmp(buf, message);
    strcpy(buf, message);
    res = strcmp(buf, message);
    strcat(buf, message);
    res = strcmp(buf, message);
    res = strlen(message);
    res = sizeof(message);

    char * ptr = strchr(message, '!');
    ptr = strrchr(message, '!');

    memset(buf, 0 ,sizeof(buf));
    res = memcmp(buf, message, sizeof(message));
    memcpy(buf, message, sizeof(message));
    res = memcmp(buf, message, sizeof(message));
    ptr = memchr(buf, '!', sizeof(buf));
    return;
}