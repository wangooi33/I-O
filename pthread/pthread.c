#include "../TureType字库/include/font.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

void show_font(int *lcd_mp, char *buf_time)
{
    // 打开字体
    font *f = fontLoad("../TureType字库/font/simkai.ttf");

    // 字体大小的设置
    fontSetSize(f, 30);

    // 创建一个画板（点阵图）宽是200个像素，高是50个像素
    bitmap *bm_time = createBitmapWithInit(220, 50, 4, getColor(0, 0, 0, 255)); // 也可使用createBitmapWithInit函数，改变画板颜色

    // 将字体写到点阵图上 0,0表示汉字在画板的左上角显示
    fontPrint(f, bm_time, 10, 10, buf_time, getColor(0, 255, 255, 255), 0);

    // 把字体框输出到LCD屏幕上  参数0,0表示画板显示的坐标位置
    show_font_to_lcd(lcd_mp, 290, 230, bm_time);

    // 关闭字体，关闭画板
    fontUnload(f);
    destroyBitmap(bm_time);
}
/*********************1********************/
pthread_t time_tid;
int stop_time_tid=0;
// 获取时间
char *get_time()
{
    time_t rawtime;
    struct tm *timeinfo;
    char *buffer = (char *)malloc(sizeof(char) * 80);
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	// 格式化时间到 buffer 字符串
	snprintf(buffer, 80, "%04d-%02d-%02d %02d:%02d:%02d",
			timeinfo->tm_year + 1900, // 年份是从1900年开始计算的，所以要加1900
			timeinfo->tm_mon + 1,     // 月份是从0开始的，所以要加1
			timeinfo->tm_mday,        // 日期
			timeinfo->tm_hour,        // 小时
			timeinfo->tm_min,         // 分钟
			timeinfo->tm_sec);        // 秒
	return buffer;
} 
void *show_time(void *arg)
{
	while(!stop_time_tid)
	{
		char *buf_time = get_time();
        show_font(p, buf_time,300,0);
		sleep(1);                // 每秒更新一次
	}
	pthread_exit(NULL);
}
/***********************2******************/
pthread_t Ctime_tid;
int stop_Ctime_tid=0;
int s = 0, m = 0, h = 0;//int n = 5;	printf("%02d\n", n);	输出为:05
void *show_Ctime(void *arg) // 计时器,pthread 线程函数需返回 void *
{
    while (!stop_Ctime_tid)
    {
        s++;
        if (s >= 60)
        {
            m++;
            s = 0;
        }
        if (m >= 60)
        {
            h++;
            m = 0;
        }
        // 构造时间字符串，例如 "01:02:03"
        char Ctime_buf[16];
        sprintf(Ctime_buf, "%02d:%02d:%02d", h, m, s);

        // 清除旧时间显示区域，防止重影
        Clean_Area(650, 75, 650 + 120, 75 + 40, 0xFFFFFF);
        // 显示新时间
        show_font(p, Ctime_buf, 650, 75);
        sleep(1); // 每秒更新一次
    }
    pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
    int lcd_fd=open("/dev/fb0",_O_RDWR);
    int *lcd_mp = (int *)mmap(
								NULL,
								800*480*4,
								PROT_READ|PROT_WRITE,
								MAP_SHARED,
								lcd_fd,
								0
							 );
    // 启动线程（只创建一次）
	pthread_create(&time_tid, NULL, show_time, NULL);
	pthread_create(&id_tid, NULL, show_Ctime, NULL);  
    
    return 0;
}