
CFLAGS:= -m32	#32位程序
CFLAGS+= -Qn	#去掉gcc版本信息
# CFLAGS+= -fno-builtin	#不要gcc内置函数
# CFLAGS+= -nostdinc	#不要标准头文件
CFLAGS+= -fno-pic	#不要位置无关的代码 positin independent code
# CFLAGS+= -fno-pie	#不要位置无关的可执行程序 positin independent executabel
# CFLAGS+= -nostdlib	#不要标准库
# CFLAGS+= -fno-stack-protector	#不要栈保护
CFLAGS+= -mpreferred-stack-boundary=2	# 不需要栈对齐
CFLAGS+= -fno-asynchronous-unwind-tables #不需要CFI信息
# CFLAGS+= -fomit-frame-pointer	# 不需要栈帧

CFLAGS:= $(strip ${CFLAGS})

.PHONY: hello.s
hello.s: hello.c
	gcc $(CFLAGS) -S $< -o $@

.PHONY: params.s
params.s: params.c
	gcc $(CFLAGS) -S $< -o $@

.PHONY: clean
clean:
	rm *.s
	rm *.o
	rm *.out