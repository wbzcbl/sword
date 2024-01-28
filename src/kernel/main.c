#include <sword/sword.h>
#include <sword/types.h>
#include <sword/io.h>
#include <sword/string.h>
#include <sword/console.h>
#include <sword/printk.h>
#include <sword/assert.h>
#include <sword/debug.h>
#include <sword/global.h>
#include <sword/task.h>
#include <sword/interrupt.h>
#include <sword/stdlib.h>

void kernel_init(){
    console_init();
    gdt_init();
    interrupt_init();
    // task_init(); 

    asm volatile(
        "sti\n"
        "movl %eax, %eax\n"
    ); // sti 打开中断

    u32 counter = 0;
    while(true){
        DEBUGK("looping in kernel init %d...\n", counter++);
        delay(10000000);
    }
    return;
}