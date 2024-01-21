#include <sword/sword.h>
#include <sword/types.h>
#include <sword/io.h>
#include <sword/string.h>
#include <sword/console.h>
#include <sword/printk.h>


void kernel_init(){
    console_init();

    int cnt =20;
    while(cnt--){
        printk("hello sword %d %d\n", cnt, -10);
    }
    return;
}