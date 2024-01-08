[bits 32]

section .text; 代码段

global inb; 将inb导出，不导出可重定位文件没该符号，影响链接
inb:
    push ebp
	mov	ebp, esp; 保存帧

    xor eax, eax;   将eax清空
    mov edx, [ebp + 8]; port
    in al, dx;  将端口号 dx 的 8bit 输入到ax

    jmp $+2;    延迟
    jmp $+2;    延迟
    jmp $+2;    延迟

    leave; 恢复栈帧
    ret

global outb; 将outb导出，不导出可重定位文件没该符号，影响链接
outb:
    push ebp
	mov	ebp, esp; 保存帧

    mov edx, [ebp + 8]; port
    mov eax, [ebp + 12]; value
    out dx, al; 将al 的 8bit 输出到端口号 dx

    jmp $+2;    延迟
    jmp $+2;    延迟
    jmp $+2;    延迟

    leave; 恢复栈帧
    ret

global inw;
inw:
    push ebp
	mov	ebp, esp; 保存帧

    xor eax, eax;   将eax清空
    mov edx, [ebp + 8]; port
    in ax, dx;  将端口号 dx 的 8bit 输入到ax

    jmp $+2;    延迟
    jmp $+2;    延迟
    jmp $+2;    延迟

    leave; 恢复栈帧
    ret

global outw;
outw:
    push ebp
	mov	ebp, esp; 保存帧

    mov edx, [ebp + 8]; port
    mov eax, [ebp + 12]; value
    out dx, ax; 将al 的 8bit 输出到端口号 dx

    jmp $+2;    延迟
    jmp $+2;    延迟
    jmp $+2;    延迟

    leave; 恢复栈帧
    ret