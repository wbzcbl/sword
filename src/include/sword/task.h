#ifndef SWORD_TASK_H
#define SWORD_TASK_H


#include <sword/types.h>

typedef u32 target_t();

typedef struct task_t
{
    u32 *stack; //内核栈
} task_t;


typedef struct task_frame_t //进程切换前后保持不变的寄存器
{
    u32 edi;
    u32 esi;
    u32 ebx;
    u32 ebp;
    void (*eip)(void); //x86-32 eip寄存器存放下一条指令内存地址
}task_frame_t;

void task_init();

#endif 