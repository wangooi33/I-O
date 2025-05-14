#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./include/font.h"
#include "./include/font_c.h"
int main(int argc,char *argv[])
{
    //1.打开LCD
    int lcd_fd=open("/dev/fb0",O_RDWR);
    //2.内存映射
    int lcd_mp=mmap(
                    NULL,
                    800*400*4,
                    PROT_READ|PROT_WRITE,
                    MAP_SHARED,
                    lcd_fd,
                    0
                    );
    //3.打开字库
    font *f=fontload("./font/simkai.ttf");
    //4.设置字体大小
    fontSetSize(f,72);
    //5.创建画布（点阵图）
    bitmap *bm=createBitmap(200,200,4,getColor(0,255,255,255));
    char buf[]="晚安";
    //6.将字体写到点阵图上
    fontPrint(f,bm,0,0,buf,getColor(0,255,255,255),0);
    //7.把字体框输出到LCD屏幕上
    show_font_to_lcd(lcd_mp,0,0,bm);
    
    Display_characterX(100,100,&buf,0x000000,2);
    return 0;
}