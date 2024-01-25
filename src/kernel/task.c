#include <sword/task.h>
#include <sword/printk.h>
#include <sword/debug.h>
#include <sword/assert.h>
#include <sword/string.h>
#include <sword/global.h>

#define PAGE_SIZE 0x1000 //页大小4k

task_t *a = (task_t*)0x1000; // 任务开始地址, 进程所占页内容包括：栈顶、栈、PCB（在页底部）
task_t *b = (task_t*)0x2000;

extern void task_switch(task_t *next);

task_t* running_task() //当前task位置
{
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n"
    ); // 将esp的内容与立即数相与，得到进程页开始的位置，函数返回是用eax返回
}

void schedule(){
    task_t *current = running_task();
    task_t *next = current == a ? b : a;
    task_switch(next);
}

u32 thread_a(){
    while(true){
        printk("A");
        schedule();
    }
}

u32 thread_b(){
    while(true){
        printk("B");
        schedule();
    }
}

static void task_create(task_t* task, target_t target){
    u32 stack = (u32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t* frame = (task_frame_t*)stack;
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void*)target;

    task->stack = (u32*)stack;
}

void task_init(){
    task_create(a, thread_a);
    task_create(b, thread_b);

    schedule();
    return;
}