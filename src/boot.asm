[org 0x7c00]

; 设置屏幕模式为文本模式，清除屏幕
mov ax, 3
int 0x10

; 初始化段寄存器
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

; 文本显示
mov ax, 0xb800
mov bx, ax
mov byte [0], 'H'

;阻塞
jmp $

;中间字节填充0
times 510 - ($ - $$) db 0

;bios要求最后两字节必须示0x55，0xaa
db 0x55, 0xaa
