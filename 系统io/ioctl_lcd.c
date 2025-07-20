/*
 * （待补全）⬇ 
 *
 * 固定参数（struct fb_fix_screeninfo）：
 * 	不会随着用户操作而改变，
 * 	| 字段名        | 含义                                 		   
 *	| ------------- | ---------------------------------- 		   
 *	| `id`          | 标识设备的名称（如 "STM32-LCD"）              
 *	| `smem_start`  | 显存的物理起始地址（只读）                    
 *	| `smem_len`    | 显存大小（字节数）                          	
 *	| `type`        | 帧缓冲类型（如 FB\_TYPE\_PACKED\_PIXELS）  	 
 *	| `visual`      | 可视类型（如 FB\_VISUAL\_TRUECOLOR 表示真彩）  
 *	| `line_length` | 一行占用的字节数                              
 *	| `xpanstep`    | 横向平移步长                                  
 *	| `ypanstep`    | 纵向平移步长                                  
 * 	| `ywrapstep`   | y方向回绕的步长                               
 *	| `accel`       | 加速卡类型（一般不用）                         
 *
 * 可变参数（struct fb_var_screeninfo）：
 * 	可通过 ioctl 系统调用设置或读取，
 * 	| 字段名                         | 含义                             
 *	| ------------------------------ | ------------------------------ 
 *	| `xres`                         | 可见区域的水平分辨率（像素）                
 *	| `yres`                         | 可见区域的垂直分辨率（像素）                
 *	| `xres_virtual`                 | 虚拟屏幕的水平分辨率                     
 *	| `yres_virtual`                 | 虚拟屏幕的垂直分辨率                     
 *	| `bits_per_pixel`               | 每个像素的位数（如 16、24、32）            
 *	| `red`, `green`, `blue`         | 各颜色分量在像素数据中的偏移和长度              
 *	| `activate`                     | 显示模式的刷新方式（如 FB\_ACTIVATE\_NOW）
 *	| `pixclock`                     | 像素时钟（单位 ps）                    
 *	| `left_margin`, `right_margin`  | 行同步信号的前后肩宽度                    
 *	| `upper_margin`, `lower_margin` | 帧同步信号的前后肩宽度                    
 *	| `hsync_len`, `vsync_len`       | 行同步和场同步信号的持续时间                 
*/


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