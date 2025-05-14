/**
  * @file name:bmp_info.c
  * @brief    :返回图片的宽、高、大小
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
    //1.定义BMP大小，高，宽
    int bmp_size=0;
    int bmp_hight=0;
    int bmp_width=0;

    //2.利用系统调用open()打开BMP图片
    int bmp_fd=open(argv[1],O_RDWR);//打开成功返回文件描述符（数组下标），失败返回-1
    if(-1==bmp_fd){
        printf("open %s is error\n",argv[1]);
        return -1;
    }

    //3.读取信息
    lseek(bmp_fd,2,SEEK_SET);
    read(bmp_fd,&bmp_size,4);

    lseek(bmp_fd,18,SEEK_SET);
    read(bmp_fd,&bmp_width,4);

    lseek(bmp_fd,22,SEEK_SET);
    read(bmp_fd,&bmp_hight,4);

    //4.输出信息
    printf("bmp name is %s\n width =%d\n higth =%d ,size =%d\n",
            argv[1],bmp_width,bmp_hight,bmp_size);
    
    return 0;
}