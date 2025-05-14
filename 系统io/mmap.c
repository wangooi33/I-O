/**
  * @file name:color_mmap.c
  * @brief    :用红色填满LCD屏幕
  * @author   :i33akq@163.com
  * @date     :2025/04/21
  * @version  :1.0
  * @note     :使用内存映射方式
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/
//假设让LCD显示红色,这个程序需要在开发板中运行 需要交叉编译 arm-linux-gcc
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
	//1.打开LCD  设备文件路径 "/dev/fb0"  O_RDWR
	int lcd_fd = open("/dev/fb0",O_RDWR);
	if (-1 == lcd_fd)
	{
		printf("open lcd error\n");
		return -1;
	}

	//2.对LCD进行内存映射  mmap函数 记得接收返回值
	int *lcd_mp = (int *)mmap(
								NULL,
								800*480*4,
								PROT_READ|PROT_WRITE,
								MAP_SHARED,
								lcd_fd,
								0
							 );

	if (MAP_FAILED == lcd_mp)
	{
		printf("mmap for lcd is error\n");
		return -1;
	}
	

	//3.把颜色分量写入到申请的映射内存空间中，通过地址偏移实现访问
	for (int i = 0;i < 800*480; ++i)
	{
		lcd_mp[i] = 0x00FF0000;
	}

	
	//4.关闭LCD并解除内存映射
	munmap(lcd_mp,800*480*4);
	close(lcd_fd);

	return 0;
}