[bits 32]

extern exit

test:
    push $; $表示当前这一行
    ret

global main
main:
    ; push 5
    ; push eax

    ; pop ebx
    ; pop ecx

    ; pusha; 将8个基础寄存器入栈

    ; popa; 将7个寄存器出栈，不包括esp（栈顶指针寄存器）

    call test
    
    push 0; 传递参数
    call exit
    

