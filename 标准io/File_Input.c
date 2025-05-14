/**
  * @file name:File_Input.c
  * @brief    :在本地磁盘打开一个存储少量数据的文本demo.txt，
  *            利用fgetc函数把文本中的字符输出到屏幕，当文本中所有字符都输出完成后就结束程序。
  * @author   :i33akq@163.com
  * @date     :2025/04/20
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reseverd
*/

#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    FILE *file=fopen("demo.txt","r");

    if(NULL==file){
        perror("open false");
        return -1;
    }

    int ch=fgetc(file);//注意返回类型
    while(ch !=EOF){
        printf("%c",ch);
        ch=fgetc(file);
    }

    fclose(file);
    return 0;
}