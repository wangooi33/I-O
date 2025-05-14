/**
  * @file name:Timed_WriteTime.c
  * @brief    :设计程序，获取当前系统时间，把时间转换为特定格式”yy年mm月dd日 星期x tt:mm:ss”，
  *            并每隔1s写入到本地磁盘中一个叫做log.txt的文本中，如果文本不存在则创建
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reseverd
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // for sleep()

int main()
{
    FILE *file = fopen("log.txt", "a"); // 打开文件用于追加写入
    if (file == NULL)
    {
        perror("Failed to open log.txt");
        return 1;
    }

    char buffer[128];

    while (1)
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);//rawtime=time(NULL);
        timeinfo = localtime(&rawtime);

        // 格式化时间：年-月-日 星期x 时:分:秒
        strftime(buffer, sizeof(buffer), "%y年%m月%d日 星期%u %H:%M:%S\n", timeinfo);

        fputs(buffer, file);
        fflush(file);  // 刷新缓冲区，确保及时写入

        sleep(1);
    }

    fclose(file);
    return 0;
}
