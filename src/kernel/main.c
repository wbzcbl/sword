#include <sword/sword.h>
#include <sword/types.h>
#include <sword/io.h>
#include <sword/string.h>
#include <sword/console.h>
#include <sword/printk.h>
#include <sword/assert.h>
#include <sword/debug.h>
#include <sword/global.h>


void kernel_init(){
    console_init();
    gdt_init();
    
    return;
}