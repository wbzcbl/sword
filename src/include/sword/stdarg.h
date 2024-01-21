#ifndef SWORD_STDARG_H
#define SWORD_STDARG_H

typedef char* va_list; //保存可变参数指针

#define stck_size(t) (sizeof(t*)>sizeof(char*))?(*(t *)((ap += sizeof(t*)) - sizeof(t*))):(*(t *)((ap += sizeof(char *)) - sizeof(char*)))
#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *)) //启用可变参数
#define va_arg(ap, t) (*(t *)((ap += sizeof(char *)) - sizeof(char*))) //获取下一个参数
#define va_end(ap) (ap = (va_list)0) //结束可变参数

#endif