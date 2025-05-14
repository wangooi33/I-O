#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

//要求：把指定路径下的规则文件的文件名称输出到终端
int main(int argc, char const *argv[])
{
	//1.检查参数有效性
	if (2 != argc)
	{
		printf("argument is invaild!\n");
		return -1;
	}
	//2.打开指定目录 opendir  错误处理  
	DIR * dir = opendir(argv[1]);
	//3.循环读取目录中的目录项，当readdir的返回值是NULL则终止循环
	while(1)
	{
		struct dirent * pdir = readdir(dir);//readdir读完之后下一次读取后面的目录指针
		if(NULL == pdir)
		{
			break;
		}

		//如果文件不是规则文件，则跳过
		if (pdir->d_type != DT_REG || pdir->d_name[0] == '.')
										//去除隐藏文件
		{
			continue;
		}
		//输出每个目录项中记录的磁盘中规则文件的名称
		printf("file name is [%s]\n",pdir->d_name); 
	}
	
	return 0;
}