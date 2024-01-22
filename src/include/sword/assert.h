#ifndef SWORD_ASSERT_H
#define SWORD_ASSERT_H

void assertion_failure(char *exp, char *file, char *base, int line);

#define assert(exp) \
    if (exp)        \
        ;           \
    else            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__) // 表示出错在哪个文件的第几行

void panic(const char *fmt, ...); // 极端情况下， 系统出错

#endif
