/**
  * @file name:File_CalSize.c
  * @brief    :利用标准io函数接口实现计算一个本地磁盘某个文件的大小，
  *            要求文件名称通过命令行进行传递，并验证是否正确。
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :使用（ls -l）验证
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reseverd
*/
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    if(2!=argc){
        printf("argument is invaile");
        exit(1);
    }

    FILE *fp=fopen(argv[1],"rb");
    if(NULL==fp){
        printf("open false");
        exit(1);
    }

    fseek(fp,0,SEEK_END);
    printf("FileSize=%d",ftell(fp));
    return 0;
}

/*
int main(int argc,char *argv[])
{
    if(2!==argc)
    {
        printf("argument error");
        return -1;
    }
    FILE *fp=fopen(argv[1],"rb");
    if(NULL==fp)
    {
        pritnf("fopen is failed");
        return -2;
    }
    int cnt=0;
    while(fgetc(fp)!=EOF)//自动偏移
    {
        cnt++;
    }
    pritnf("file size is %d",cnt);
    return 0;
}










*/
