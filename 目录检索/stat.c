#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

// 要求：检查一个路径是否为字符设备
int main(int argc, char const *argv[])
{
    // 1. 检查参数个数
    if (argc != 2)
    {
        printf("Usage: %s <device_path>\n", argv[0]);
        return -1;
    }

    const char *dev_name = argv[1];
    struct stat st;

    // 2. 使用 stat 获取文件信息
    if (-1 == stat(dev_name, &st))
    {
        fprintf(stderr, "Cannot identify '%s': %d, %s\n",
                dev_name, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 3. 判断是否为字符设备
    if (!S_ISCHR(st.st_mode))
    {
        fprintf(stderr, "%s is no device\n", dev_name);
        exit(EXIT_FAILURE);
    }

    // 4. 确认输出
    printf("'%s' is a character device.\n", dev_name);
    return 0;
}
