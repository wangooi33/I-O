
//Compression:压缩（编码），把bmp的颜色分量写入到jpg中

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */

#include "jpeglib.h"

void write_JPEG_file (char * filename, int quality)
{
	/* This struct contains the JPEG compression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	* It is possible to have several such structures, representing multiple
	* compression/decompression processes, in existence at once.  We refer
	* to any one struct (and its associated working data) as a "JPEG object".
	*/
	struct jpeg_compress_struct cinfo;
	/* This struct represents a JPEG error handler.  It is declared separately
	* because applications often want to supply a specialized error handler
	* (see the second half of this file for an example).  But here we just
	* take the easy way out and use the standard error handler, which will
	* print a message on stderr and call exit() if compression fails.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	/* Step 1: allocate and initialize JPEG compression object */

	/* We have to set up the error handler first, in case the initialization
	* step fails.  (Unlikely, but it could happen if you are out of memory.)
	* This routine fills in the contents of struct jerr, and returns jerr's
	* address which we place into the link field in cinfo.
	*/
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	/* Here we use the library-supplied code to send compressed data to a
	* stdio stream.  You can also write your own code to do something else.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to write binary files.
	*/
	if ((outfile = fopen(filename, "wb")) == NULL) {
	fprintf(stderr, "can't open %s\n", filename);
	exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	/* Step 3: set parameters for compression */

	/* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	* (You must set at least cinfo.in_color_space before calling this,
	* since the defaults depend on the source color space.)
	*/
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	* Here we just illustrate the use of quality (quantization table) scaling:
	*/
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	* Pass TRUE unless you are very sure of what you're doing.
	*/
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	* To keep things simple, we pass one scanline per call; you can pass
	* more if you wish, though.
	*/
	row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
	/* jpeg_write_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could pass
		* more than one scanline at a time if that's more convenient.
		*/
	row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
	(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	/* And we're done! */
}

void write_JPEG_file(const char *filename, int width, int height, unsigned char *rgb_buffer, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *outfile;
	JSAMPROW row_pointer[1];
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);

	jpeg_start_compress(&cinfo, TRUE);
	row_stride = width * 3;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
}

/*
 * 1.注意:JPEG 压缩过程保持图像分辨率不变（比如你给它1920×1080，它压缩后仍是1920×1080像素）。
 *      quality 参数控制的是量化表的“严格程度”，即压缩时对频率系数的舍弃多少。
 * 
 * 
 * 2.jpeg_set_quality(&cinfo, quality, TRUE);
 *   范围： 1 到 100
 *
 *   建议：
 *   100：几乎无损压缩（非常高质量，文件大）
 *   90~95：高质量，肉眼几乎无差别
 *   70~85：中等质量（常用于网页图片）
 *   < 50：明显压缩，肉眼可察觉失真
 *   10~20：极强压缩，适合缩略图预览
 * 
 *   质量与体积对比示例（同一张 1920x1080 图片）:
 *   | Quality | 文件大小| 肉眼观感      |
 *   | ------- | ------ | -------       |
 *   | 100     | 3.5 MB | 与原图无差别   |
 *   | 90      | 1.2 MB | 细节清晰       |
 *   | 75      | 600 KB | 略有模糊       |
 *   | 50      | 350 KB | 可见失真       |
 *   | 20      | 150 KB | 像素块明显模糊  |
 *
 * 
 * 3.技术原理（简化理解）：
 *   JPEG 使用一个量化表（quantization table）来降低 DCT 频率系数的精度。
 *   quality 控制这个量化表的缩放：
 *   高质量 ➜ 缩放少 ➜ 保留更多高频细节 ➜ 文件大
 *   低质量 ➜ 缩放多 ➜ 丢弃高频 ➜ 文件小但失真
 *
 * 
 * 4.DCT（离散余弦变换）:DCT 将图像的空间信息（像素值）转化为频率信息，从而能够在压缩过程中丢弃一些不太重要的高频信息，从而达到压缩效果。
*/

int main(int argc, char const *argv[])
{
	//文件信息的读取
    /*
    lseek(bmp_fd,2,SEEK_SET);
    read(bmp_fd,&bmp_size,4);

    lseek(bmp_fd,18,SEEK_SET);
    read(bmp_fd,&bmp_width,4);

    lseek(bmp_fd,22,SEEK_SET);
    read(bmp_fd,&bmp_hight,4);
	*/
	int width, height;
    unsigned char *rgb_data;

    if (read_bmp("input.bmp", &width, &height, &rgb_data) != 0) {
        fprintf(stderr, "Failed to read BMP\n");
        return -1;
    }

    write_JPEG_file("output.jpg", width, height, rgb_data, 90);

    free(rgb_data);
    return 0;
}
