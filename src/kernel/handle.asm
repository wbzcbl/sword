[bits 32]
; 中断处理函数入口

section .text

extern printk

global interrupt_handler
interrupt_handler:
    xchg bx, bx
    
    push message
    call printk
    add esp, 4 ;调用完成之后回复栈

    xchg bx, bx
    iret ; 中断返回指令，执行之后段寄存器CS、偏移寄存器IP(32位x86机器中位eip寄存器)、标志寄存器EFLAGS回复终端执行之前的数据

section .data

message:
    db "defult interrupt", 10, 0
