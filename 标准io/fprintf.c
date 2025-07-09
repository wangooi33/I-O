#include <stdio.h>
int main(int argc,char *argv[])
{
    FILE *fp=fopen("xxx.txt","rb");
    if(NULL==fp)
    {
        perror("fopen is failed");
        exit(1);
    }

    int a=1;
    fprintf(fp,"a=%d",a);//到文件

    fclose(fp);
    return 0;
}