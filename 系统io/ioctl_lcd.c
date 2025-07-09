/**
  * @file name:Color.c
  * @brief    :设计程序，利用ioctl函数获取LCD的硬件参数，
  *            把LCD屏幕的宽和高以及色深的位数输出到屏幕，
  *            下载程序到开发板并进行验证。
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :必须包含该头文件 <linux/fb.h> !!!!
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <liunx/fb.h>   //ioctl
                        /*
                          在Linux系统下是利用Framebuffer子系统来驱动显示设备的，
                          所以Framebuffer子系统会读取LCD屏的硬件信息并存储在内核空间中，
                          关于LCD的硬件参数都是定义在/usr/include/linux/fb.h头文件中。
                        
                        */

int main(int argc,char *argv[])
{
	int lcd_fl=open("dev/fd0",O_RDWR);

	struct fb_fix_screeninfo lcd_finfo;//主要用于获取FrameBuffer的固定参数
	struct fb_var_screeninfo lcd_vinfo;//主要用于获取和设置FrameBuffer的可变（虚拟）屏幕参数

	//请求信息去看:/usr/include/linux/fb.h
	ioctl(lcd_fl,FBIOGET_VSCREENINFO,&lcd_vinfo);

	printf("lcd width=%d\n",lcd_vinfo.xres);
	printf("lcd hight=%d\n",lcd_vinfo.yres);
	printf("lcd bpp=%d\n",lcd_vinfo.bit_per_pixel);

	close(lcd_fl);
	return 0;
}