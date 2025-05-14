#include <stdio.h>
int main(int argc,char *argv[])
{
    char buf[128]={0};
    int b=2;
    sprintf(buf,"b=%d",b);//向缓冲区中输出格式化字符串
    printf("%s\n", buf); //输出b=2,自动加入'\0'
    return 0;
}