[org 0x1000]; 实模式内存布局要求，这段位置属于可用区域

dw 0x55aa; 魔术，用于判断错误

; 打印字符串
mov si, loading
call print

; xchg bx, bx

detect_memory:
    xor ebx, ebx; ebx置0

    ; es：di 结构体的缓存位置
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer

    mov edx, 0x534d4150

.next:
    ; 子功能号
    mov eax, 0xe820
    ; ards 结构的大小
    mov ecx, 20
    ; 调用0x15 系统调用
    int 0x15

    ; 如果CF置位，则表示出错
    jc error
    ; 没出错，缓存指针指向下一个结构体
    add di, cx
    inc word [ards_count]

    cmp ebx, 0
    jnz .next

    mov si, detecting
    call print

    jmp prepare_protected_mode

prepare_protected_mode:
    xchg bx, bx
    cli; 关闭中断

    ; 打开 A20 线，地址回绕
    in al, 0x92
    or al, 0b10
    out 0x92, al

    lgdt [gdt_ptr]; 加载gdt

    mov eax, cr0
    or eax, 1
    mov cr0, eax; 启动保护模式

    ; 用跳转刷新缓存，启用保护模式
    jmp dword code_selector:protect_mode

print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

loading:
    db "Loading sword...",10 ,13, 0; \n\r
detecting:
    db "Detecting Memory Success...",10, 13, 0

error:
    mov si, .msg
    call print
    hlt; 载入终止
    jmp $
    .msg db "Booting error!!!", 10, 13, 0


[bits 32]
protect_mode:
    ;xchg bx, bx
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax; 初始化段寄存器

    mov esp, 0x10000; 修改栈顶

    mov edi, 0x10000; 读取目标内存
    mov ecx, 10; 起始扇区
    mov bl, 200; 扇区数量
    call read_disk

    jmp dword code_selector:0x10000

    ud2; 正常不会执行到这，表示出错

jmp $; 阻塞

read_disk:
    ; 设置读写扇区数量
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx; 0x1f3
    mov al, cl; 起始扇区前8位
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; 起始扇区中8位
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; 起始扇区高8位
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; 高四位置0

    mov al, 0b1110_0000;
    or al, cl
    out dx, al; 主盘 lba模式

    inc dx; 0x1f7
    mov al, 0x20; 读硬盘
    out dx, al

    xor ecx, ecx; 清空ecx
    mov cl, bl; 得到读写扇区的数量

    .read:
        push cx; 保存cx
        call .waits; 等待数据 
        call .reads; 读取一个扇区
        pop cx; 回复cx
        loop .read
    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al,dx
            jmp $+2; nop 直接跳转下一行
            jmp $+2; jmp时间消耗更高，加点延迟
            jmp $+2
            and al, 0b1000_1000
            cmp al, 0b0000_1000
            jnz .check
        ret
    
    .reads:
        mov dx, 0x1f0
        mov cx, 256; 一个扇区256个字

        .readw:
            in ax, dx
            jmp $+2
            jmp $+2
            jmp $+2
            mov [edi], ax
            add edi, 2
            loop .readw
        ret

code_selector equ (1<<3); 代码段段选择子 RPL 0 - 全局描述符 - 第一段
data_selector equ (2<<3); 数据段段选择子 RPL 0 - 全局描述符 - 第二段

memory_base equ 0; 内存开始位置，基地址

memory_limit equ ((1024 * 1024 * 1024 * 4) / (1024 * 4)) - 1; 内存界限 4G / 4K - 1，粒度为4kB

gdt_ptr:
    dw (gdt_end - gdt_base) - 1
    dd gdt_base
gdt_base:
    dd 0, 0; NULL 描述符
gdt_code:; 代码段内存描述符代码，用来表示内存位置、大小、属性
    dw memory_limit & 0xffff; 段界限0~15位
    dw memory_base & 0xffff; 基地址0~15位
    db (memory_base >> 16) & 0xff; 基地址16~23位
    db 0b_1_00_1_1_0_1_0; 存在 - dlp 0 - S _ 代码 - 非依从 - 可读 - 没有被访问过
    db 0b_1_1_0_0_0000 | (memory_limit >> 16) & 0xf; 4K - 32位 - 不是64位 - 段界限 16~19
    db (memory_base >> 24) & 0xff; 基地址24~31位
gdt_data:; 数据段内存描述符代码，用来表示内存位置、大小、属性
    dw memory_limit & 0xffff; 段界限0~15位
    dw memory_base & 0xffff; 基地址0~15位
    db (memory_base >> 16) & 0xff; 基地址16~23位
    db 0b_1_00_1_0_0_1_0; 存在 - dlp 0 - S _ 数据 - 向上 - 可写 - 没有被访问过
    db 0b_1_1_0_0_0000 | (memory_limit >> 16) & 0xf; 4K - 32位 - 不是64位 - 段界限 16~19
    db (memory_base >> 24) & 0xff; 基地址24~31位
gdt_end:

ards_count:
    dw 0
ards_buffer:
