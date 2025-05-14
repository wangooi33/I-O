#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
	//1.打开LCD   open  
	int lcd_fd = open("/dev/fb0",O_RDWR);


	//2.对LCD进行内存映射  mmap
	lcd_mp = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);

	//3.显示开机动画
	char gif_path[128] ={0}; 

	while(1)
	{
		for (int i = 0; i < 126; ++i)
		{
			sprintf(gif_path,"./ovo/Frame%d.jpg",i);  //构造jpg图片的路径
			read_JPEG_file (gif_path,0,0);			  //在LCD上显示
			usleep(1000*20);						  //FPS = 50HZ
		}	
	}
	
	return 0;
}