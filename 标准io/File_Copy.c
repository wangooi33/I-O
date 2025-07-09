/**
  * @file name:File_Copy.c
  * @brief    :利用标准IO函数接口实现文件拷贝，把本地磁盘的文件A中的数据完整的拷贝到另一个文本B中，
  *            如果文本B不存在则创建，要求文本A的名称和文本B的名称通过命令行传递，并进行验证是否正确。
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :使用（ls -l）验证
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reseverd
*/

#include <stdio.h>
#include <stdlib.h>

#define BUFFERSIZE 512

int main(int argc,char const *argv[])
{
    //1.文件参数
    if(3!=argc){
        printf("argument is invaild");
        exit(1);
    }
    //2.打开文件，并进行错误处理
    FILE *src_fp=fopen(argv[1],"rb");//待拷贝文件
	//b：二进制方式打开，不会对特殊字符进行解释，而是作为普通字符

    if(NULL==src_fp){
        printf("fopen %s is errer",argv[1]);
        exit(1);
    }
    FILE *dest_fp=fopen(argv[2],"wb");//目标文件
    if(NULL==dest_fp){
        printf("fopen %s is errer",argv[2]);
        exit(1);
    }
    //3.定义缓冲区存数据
    char data_buffer[BUFFERSIZE] = {0};

    //4.偏移光标到尾为了计算大小，再偏移光标到头
    fseek(src_fp,0,SEEK_SET);
    int src_size=ftell(src_fp);
    fseek(src_fp,0,SEEK_SET);

    //5.计算读取数据块次数
    int loop_cnt=src_size/BUFFERSIZE;
    int remainder=src_size%BUFFERSIZE;

    //6.循环写入目标文件
    while(loop_cnt--){
        //读取
        fread(data_buffer,BUFFERSIZE,1,src_fp);
        //写入
        fwrite(data_buffer,BUFFERSIZE,1,dest_fp);
    }
    
    //7.若有剩余，清空缓冲区，再写入
    if(remainder>0){
        //清空
        bzero(data_buffer);
        //读取
        fread(data_buffer,remainder,1,src_fp);
        //写入
        fwrite(data_buffer,remainder,1,dest_fp);
    }

    //8.为了验证是否拷贝完成，额统计目标文件的数据量
    printf("dest_fl  size = %d",ftell(dest_fp));

    //9.关闭文件
    fclose(src_fp);
    fclose(dest_fp);

    return 0;
}