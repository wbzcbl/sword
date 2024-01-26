#include <sword/interrupt.h>
#include <sword/global.h>
#include <sword/debug.h>

gate_t idt[IDT_SIZE]; // 中断描述符表
pointer_t idt_ptr; // 中断描述符表指针，和全局描述符指针相同构造

extern void interrupt_handler();

//中断初始化
void interrupt_init()
{
    for (size_t i = 0; i < IDT_SIZE; i++)
    {
        gate_t *gate = &idt[i];

        gate->offset0 = (u32)interrupt_handler & 0xffff;
        gate->offset1 = ((u32)interrupt_handler >> 16) & 0xffff;
        gate->selector = 1 << 3; // 代码段
        gate->reserved = 0;      // 保留不用
        gate->type = 0b1110;     // 中断门
        gate->segment = 0;       // 系统段
        gate->DPL = 0;           // 内核态
        gate->present = 1;       // 有效
    }
    idt_ptr.base = (u32)idt;
    idt_ptr.limit = sizeof(idt) - 1;
    BMB;
    
    asm volatile("lidt idt_ptr\n");
}
