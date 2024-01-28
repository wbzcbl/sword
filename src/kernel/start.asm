[bits 32]

extern kernel_init

global _start
_start:
    call kernel_init
    xchg bx, bx
    int 0x10 ;调用中断函数0x80，系统调用

    mov bx, 0
    div bx

    jmp $; 阻塞