#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>

//获取x，y坐标
void gets_pos(int *x,int *y)
{
	//读取触摸屏数据，获取坐标
	struct input_event buf;
	while(1)
	{
		read(ts_fd,&buf,sizeof(buf));
		if(buf.type == EV_ABS) //触摸屏事件
		{
			if(buf.code == ABS_X) //x轴
			{
				*x = buf.value*800/1024;
			}
			if(buf.code == ABS_Y) //Y轴
			{
				*y = buf.value*480/600;
			}
		}
		//判断手指松开
		if(buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0) break;
	}
}

int main(int argc,char *argv[])
{
	//1.打开LCD屏
	int ts_fd=open("/dev/input/event0",O_RDWR);
	int x,y;
	while(1)
	{
		gets_pos(&x,&y);
		printf("(%d,%d)\n",&x,&y);
	}
	return 0;
}