/**
  * @file name:Bmp_Zoom.c
  * @brief    :把一张任意尺寸的BMP图片等比例且不失真的缩小为原来的1/2，
  *            并生成一张新的BMP图片，要求图片的路径都通过命令行进行传递
  * @author   :i33akq@163.com
  * @date     :2025/04/21
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/

/*  
 2X2采样,取平均值防止失真

 -----          ----- -----
|     |  -->   |idx1 |idx2 |
 -----          ----- -----
               |idx3 |idx4 |
                ----- -----

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#pragma pack(1)
typedef struct {
    unsigned char  bfType[2];
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BMPHeader;

typedef struct {
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} BMPInfoHeader;
#pragma pack()

int main(int argc, char *argv[]) {
    //1.命令行参数传递判断
    if (argc != 3) {
        printf("用法: %s 输入.bmp 输出.bmp\n", argv[0]);
        return -1;
    }

    //2.打开待缩小图片
    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        perror("打开输入文件失败");
        return -1;
    }

    //3.读取图片头文件信息
    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfo;
    fread(&bmpHeader, sizeof(bmpHeader), 1, in);
    fread(&bmpInfo, sizeof(bmpInfo), 1, in);

    //4.判错
    if (bmpHeader.bfType != 0x4D42 || bmpInfo.biBitCount != 24) {
        printf("只支持24位BMP图片\n");
        fclose(in);
        return -1;
    }

    //5.分配缓冲区，每行像素数据按4字节对齐
    // 原图
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int row_bytes = ((width * 3 + 3) / 4) * 4;//公式：向上取整到4的倍数
    char *src = malloc(row_bytes * height);
    //读取原图片像素数据
    fseek(in, bmpHeader.bfOffBits, SEEK_SET); // 跳转到图像数据开始处
    fread(src, 1, row_bytes * height, in);    // 读取图像像素
    
    //新图
    int new_w = width / 2;
    int new_h = height / 2;
    int new_row_bytes = ((new_w * 3 + 3) / 4) * 4;// 缩小后图像每行字节
    unsigned char *dst = calloc(1, new_row_bytes * new_h);// 分配缩小图像缓冲区（清零防止行末出现杂色）

    //6. 将原始 BMP 图像数据 src 按 2x2 平均采样缩小为原来的一半，结果写入 dst
    for (int y = 0; y < new_h; y++) {
        int sy = y * 2;  // 原图中对应的起始行
        for (int x = 0; x < new_w; x++) {
            int sx = x * 2;  // 原图中对应的起始列

            // 每个 2x2 块的4个像素索引（BGR顺序）
            int idx1 = (sy    ) * row_bytes + (sx    ) * 3;
            int idx2 = (sy    ) * row_bytes + (sx + 1) * 3;
            int idx3 = (sy + 1) * row_bytes + (sx    ) * 3;
            int idx4 = (sy + 1) * row_bytes + (sx + 1) * 3;

            // 平均计算 BGR 三通道（确保转为 unsigned char 防止负数溢出）
            //>> 2 是右移操作，等效于将累加结果除以 4（/ 4）。因为 2 的幂次操作通常比除法更高效。
            int b = ((unsigned char)src[idx1 + 0] + (unsigned char)src[idx2 + 0] +
                    (unsigned char)src[idx3 + 0] + (unsigned char)src[idx4 + 0]) >> 2;
            int g = ((unsigned char)src[idx1 + 1] + (unsigned char)src[idx2 + 1] +
                    (unsigned char)src[idx3 + 1] + (unsigned char)src[idx4 + 1]) >> 2;
            int r = ((unsigned char)src[idx1 + 2] + (unsigned char)src[idx2 + 2] +
                    (unsigned char)src[idx3 + 2] + (unsigned char)src[idx4 + 2]) >> 2;

            // 写入缩小图像的对应位置
            int dst_idx = y * new_row_bytes + x * 3;
            dst[dst_idx + 0] = b;
            dst[dst_idx + 1] = g;
            dst[dst_idx + 2] = r;
        }
    }

    bmpInfo.biWidth = new_w;
    bmpInfo.biHeight = new_h;
    bmpInfo.biSizeImage = new_row_bytes * new_h;
    bmpHeader.bfSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + bmpInfo.biSizeImage;

    FILE *out = fopen(argv[2], "wb");
    if (!out) {
        perror("打开输出文件失败");
        free(src);
        free(dst);
        return -1;
    }

    fwrite(&bmpHeader, sizeof(bmpHeader), 1, out);
    fwrite(&bmpInfo, sizeof(bmpInfo), 1, out);
    fwrite(dst, 1, bmpInfo.biSizeImage, out);

    fclose(out);
    free(src);
    free(dst);

    printf("缩小完成: %s -> %s\n", argv[1], argv[2]);
    return 0;
}
