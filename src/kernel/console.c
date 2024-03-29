#include<sword/console.h>
#include<sword/io.h>
#include<sword/string.h>

#define CRT_ADDR_REG 0x3D4 // CRT(6845)索引寄存器
#define CRT_DATA_REG 0x3D5 // CRT(6845)数据寄存器

#define CRT_START_ADDR_H 0xC // 显示内存起始位置 - 高位
#define CRT_START_ADDR_L 0xD // 显示内存起始位置 - 低位
#define CRT_CURSOR_H 0xE     // 光标位置 - 高位
#define CRT_CURSOR_L 0xF     // 光标位置 - 低位

#define MEM_BASE 0xB8000              // 显卡内存起始位置
#define MEM_SIZE 0x4000               // 显卡内存大小
#define MEM_END (MEM_BASE + MEM_SIZE) // 显卡内存结束位置
#define WIDTH 80                      // 屏幕文本列数
#define HEIGHT 25                     // 屏幕文本行数
#define ROW_SIZE (WIDTH * 2)          // 每行字节数
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // 屏幕字节数

#define ASCII_NUL 0x00
#define ASCII_ENQ 0x05
#define ASCII_ESC 0x1B // ESC
#define ASCII_BEL 0x07 // \a
#define ASCII_BS 0x08  // \b
#define ASCII_HT 0x09  // \t
#define ASCII_LF 0x0A  // \n
#define ASCII_VT 0x0B  // \v
#define ASCII_FF 0x0C  // \f
#define ASCII_CR 0x0D  // \r
#define ASCII_DEL 0x7F

static u32 screen;  //当前显示器开始的内存位置

static u32 pos; //记录当前光标的内存位置
static u32 x, y;    //记录光标的坐标
static u8 attr = 7; //字符样式
static u16 erase = 0x0720;  //带有样式的空格（小端存储）

//获得当前显示器开始位置
static void get_screen(){
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);    //开始位置高地址
    screen = inb(CRT_DATA_REG)<<8;  //开始位置高八位
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);   //开始位置低八位
    screen |= inb(CRT_DATA_REG);

    screen <<= 1;   //screen * 2
    screen += MEM_BASE;
}

//设置当前显示器开始位置
static void set_screen(){
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);    //开始位置高地址
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);   //开始位置低八位
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 1) & 0xff);
    
}

//获得当前光标位置
static void get_cursor(){
    outb(CRT_ADDR_REG, CRT_CURSOR_H);    //光标位置高地址
    pos = inb(CRT_DATA_REG)<<8;  //位置高八位
    outb(CRT_ADDR_REG, CRT_CURSOR_L);   //光标位置低八位
    pos |= inb(CRT_DATA_REG);
    
    get_screen();
    pos <<= 1;
    pos += MEM_BASE;

    u32 delta = (pos - screen) >> 1;
    x = delta % WIDTH;
    y = delta / WIDTH;
}

//设置光标开始位置
static void set_cursor(){
    outb(CRT_ADDR_REG, CRT_CURSOR_H);    //高地址
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);   //低八位
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 1) & 0xff);
    
}

//清空屏幕
void console_clear(){
    screen = MEM_BASE;
    pos = MEM_BASE;
    x = y = 0;
    set_screen();
    set_cursor();

    u16 *ptr = (u16*)MEM_BASE;
    while(ptr < (u16*)MEM_END){
        *ptr++ = erase;
    }

}

//向上滚屏
static void scroll_up(){
    if(screen + SCR_SIZE + ROW_SIZE < MEM_END){
        u32 *ptr = (u32 *)(screen + SCR_SIZE);
        for(size_t i = 0; i < WIDTH; i++){
            *ptr++ = erase;
        }
        screen += ROW_SIZE;
        pos += ROW_SIZE;
    }else{
        memcmp((void*)MEM_BASE, (void*)screen, SCR_SIZE);
        pos -= (screen - MEM_BASE);
        screen = MEM_BASE;
    }
    set_screen();
}

static void command_lf(){
    if(y + 1 < HEIGHT){
        y++;
        pos += ROW_SIZE;
        return;
    }
    scroll_up();
}
static void command_cr(){
    pos -= (x << 1);
    x = 0;
}

static void command_bs(){
    if(x){
        x--;
        pos -= 2;
        *(u16 *)pos = erase;
    }
}

static void command_del(){
    *(u16 *)pos = erase;
}

void console_write(char *buf, u32 count){
    char ch;
    while(count--){
        ch = *buf++;
        switch (ch)
        {
        case ASCII_NUL:
            break;
        case ASCII_BEL:
            break;
        case ASCII_BS:
            command_bs();
            break;
        case ASCII_HT:
            break;
        case ASCII_LF:
            command_lf();
            command_cr();
            break;
        case ASCII_VT:
            break;
        case ASCII_CR:
            command_cr();
            break;
        case ASCII_DEL:
            command_del();
            break;
        case ASCII_ENQ:
            break;
        case ASCII_ESC:
            break;
        case ASCII_FF:
            command_lf();
            break;
        default:
            if(x >= WIDTH){
                x -= WIDTH;
                pos -= ROW_SIZE;
                command_lf();
            }
            *((char*)pos) = ch;  //字符内容
            pos++;
            *((char*)pos) = attr;    //字符样式
            pos++;

            x++;
            break;
        }
    }
    set_cursor();
}
void console_init(){
    console_clear();
}