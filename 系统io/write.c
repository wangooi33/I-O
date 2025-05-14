/**
  * @file name:Draw_Round.c
  * @brief    :Draw a solid round in screen
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    // 1. 打开 LCD
    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1) {
        perror("open lcd failed");
        exit(1);
    }

    //2.缓冲区
    int colorbuff[800*480]={0};

    //3.全屏涂黑
    for (int i = 0; i < 800 * 480; i++) {
        colorbuff[i] = 0x00000000;  // 黑色背景
    }

    //4.画圆，圆心（400,240）,r=10
    for(int y=230;y<250;y++)
    {
        for(int x=390;x<410;x++)
        {
           if(((x-400)*(x-400)+(y-240)*(y-240)) <= 100)
           {
                colorbuff[x+y*800]=0X00FF0000;
           }
        }
    }

    //4.写入
    write(lcd_fd,colorbuff,800*480*4);

    //5.关闭
    close(lcd_fd);

    return 0;
}


用宏定义实现
/*                
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 480
#define CX 400
#define CY 240
#define R 10

int main()
{
    // 打开 LCD
    int lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd == -1) {
        perror("open lcd failed");
        exit(1);
    }

    // 缓冲区
    int colorbuff[WIDTH * HEIGHT] = {0};

    // 全屏涂黑
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        colorbuff[i] = 0x00000000;
    }

    // 画圆
    for (int y = CY - R; y <= CY + R; y++) {
        for (int x = CX - R; x <= CX + R; x++) {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                if ((x - CX)*(x - CX) + (y - CY)*(y - CY) <= R*R) {
                    colorbuff[y * WIDTH + x] = 0x00FF0000;  // 红色
                }
            }
        }
    }

    // 写入
    write(lcd_fd, colorbuff, WIDTH * HEIGHT * 4);

    // 关闭
    close(lcd_fd);

    return 0;
}
*/