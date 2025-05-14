/**
  * @file name:Bmp_Show.c
  * @brief    :实现在LCD上任意位置显示一张任意大小的色深为24bit的bmp图片，
  *            要求图片不失真可以在开发板的LCD上显示
  * @author   :i33akq@163.com
  * @date     :2025/04/21
  * @version  :1.0
  * @note     :none
  * @CopyRight (C)   2024-2025   i33akq@163.com   ALL Right Reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#pragma pack(1)

typedef struct {
	unsigned short bfType;
	unsigned int   bfSize;//位图文件大小，单位字节
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffBits;//文件开始到位图数据开始之间的偏移量字节
} Bitmap_File_Header_t;

typedef struct {
	unsigned int biSize;//图像大小
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} Bitmap_Information_Header_t;

#pragma pack()

void ShowBmp_OpPosition(const char *name, int offset_x, int offset_y, int *lcd_mp) {
	FILE *bmp_fp = fopen(name, "rb");
	if (bmp_fp == NULL) {
		perror("fopen bmp");
		return;
	}

	Bitmap_File_Header_t file_header;
	fread(&file_header, sizeof(file_header), 1, bmp_fp);

	Bitmap_Information_Header_t headerinfo;
	fread(&headerinfo, sizeof(headerinfo), 1, bmp_fp);

	printf("bmp width = %d, height = %d\n", headerinfo.biWidth, headerinfo.biHeight);

	// 每行像素数据按4字节对齐
	int width = headerinfo.biWidth;
	int height = headerinfo.biHeight;

	int row_bytes = ((width * 3 + 3) / 4) * 4;//公式：向上取整到4的倍数
	char *bmp_buf = (char *)calloc(1,row_bytes * height);
	fseek(bmp_fp, file_header.bfOffBits, SEEK_SET);//偏移到图像数据处
	fread(bmp_buf, 1, row_bytes * height, bmp_fp);
	fclose(bmp_fp);

	// 写入到 LCD
	int lcd_x =0;
	int lcd_y =0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			//计算要显示的位置，相当于把图片移动到 LCD 的 (offset_x, offset_y) 起始点。
			lcd_x = offset_x + x;//向左偏移offset_x个大小
			lcd_y = offset_y + y;//向上偏移offset_y个大小
			unsigned char B = bmp_buf[(height - 1 - y) * row_bytes + x * 3 + 0];
			unsigned char G = bmp_buf[(height - 1 - y) * row_bytes + x * 3 + 1];
			unsigned char R = bmp_buf[(height - 1 - y) * row_bytes + x * 3 + 2];

			int color = (R << 16) | (G << 8) | B;

			if (lcd_x >= 0 && lcd_x < 800 && lcd_y >= 0 && lcd_y < 480) {
				lcd_mp[lcd_y * 800 + lcd_x] = color;
			}
		}
	}

	free(bmp_buf);
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Usage: %s <bmp_file> <x> <y>\n", argv[0]);
		return -1;
	}

	const char *bmp_path = argv[1];
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);

	int lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1) {
		perror("open lcd failed");
		return -1;
	}

	//内存映射
	int *lcd_mp = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	if (MAP_FAILED == lcd_mp) {
		perror("mmap failed");
		close(lcd_fd);
		return -1;
	}

	ShowBmp_OpPosition(bmp_path, x, y, lcd_mp);

	munmap(lcd_mp, 800 * 480 * 4);
	close(lcd_fd);

	return 0;
}


/*
int main(int argc, char const *argv[])
{
	//1.打开待显示的BMP图像  fopen
	FILE * bmp_fp = fopen("demo.bmp","rb");
	if (NULL == bmp_fp)
	{
		return -1;
	}

	//2.读取BMP文件的图像信息，获取BMP的宽和高
	Bitmap_Info_Header_t headerinfo;
	fseek(bmp_fp,14,SEEK_SET);
	fread(&headerinfo,1,40,bmp_fp); //读取40字节
	printf("bmp width = %d,height = %d\n",headerinfo.biWidth,headerinfo.biHeight);

	//3.读取BMP图*片的颜色分量  800*480*3
	char bmp_buf[800*480*3] = {0};
	fread(bmp_buf,1,800*480*3,bmp_fp);

	//4.关闭BMP
	fclose(bmp_fp);

	//5.打开LCD   open  
	int lcd_fd = open("/dev/fb0",O_RDWR);


	//6.对LCD进行内存映射  mmap
	int * lcd_mp = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd_fd,0);

	//7.循环的把BMP图像的颜色分量依次写入到LCD的像素点中 
	int i = 0;
	int data = 0;

	for (int y = 480-1; y >= 0; y--)
	{
		for (int x = 0; x < 800 ; ++x)
		{
			//把BMP图片的一个像素点的颜色分量转换为LCD屏幕的一个像素点的颜色分量格式  ARGB <--- BGR
			data |= bmp_buf[i];			//B
			data |= bmp_buf[i+1]<<8;	//G
			data |= bmp_buf[i+2]<<16;  	//R

			lcd_mp[800*y + x] = data;  //BGR BGR BGR .... 

			i+=3;  
			data = 0;
		}
	}
	
	//8.关闭LCD
	close(lcd_fd);
	munmap(lcd_mp,800*480*4);

	return 0;
}
*/