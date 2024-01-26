
global task_switch
task_switch:
    push ebp
    mov ebp, esp ; 保存栈帧 + 指向当前栈底

    push ebx
    push esi
    push edi ; 保存ABI需要的寄存器

    mov eax, esp
    and eax, 0xfffff000; 当前执行任务地址

    mov [eax], esp; 保存当前任务到eax所指向的地址

    mov eax, [ebp + 8]; next
    mov esp, [eax]

    pop edi
    pop esi
    pop ebx
    pop ebp

    ret

 