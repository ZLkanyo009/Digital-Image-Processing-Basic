#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned char BYTE;
//asdf1234
/*********** *********** *********** *********** *********** *********** ***********
* definition :struct
* Description :位图文件头
*********** *********** *********** *********** *********** *********** ***********/
#pragma pack(1)/////////////////将结构体中成员      按n字节对齐
typedef struct tagBITMAPFILEHEADER
{
	WORD bfType;////////////////文件类型，必须为BM
	DWORD bfSize;///////////////指定文件大小，以字节为单位（3-6字节，低位在前）
	WORD bfReserved1;///////////文件保留字，必须为0
	WORD bfReserved2;///////////文件保留字，必须为0
	DWORD bfOffBits;////////////从文件头到实际位图数据的偏移字节数（11-14字节，低位在前）
}BITMAPFILEHEADER;
/*********** *********** *********** *********** *********** *********** ***********
* definition :struct
* Description :位图信息头
*********** *********** *********** *********** *********** *********** ***********/
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;///////////////本结构所占用字节数，为40。注意：实际操作中则有44，这是字节补齐的原因
	LONG biWidth;///////////////位图的宽度，以像素为单位
	LONG biHeight;//////////////位图的高度，以像素为单位
	WORD biPlanes;//////////////目标设备的级别，必须为1
	WORD biBitCount;////////////每个像素所需的位数，1（双色），4(16色），8(256色）16(高彩色)，24（真彩色）或32之一
	DWORD biCompression;////////位图压缩类型，0（不压缩），1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	DWORD biSizeImage;//////////位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位
	LONG biXPelsPerMeter;///////位图水平分辨率，每米像素数
	LONG biYPelsPerMeter;///////位图垂直分辨率，每米像素数
	DWORD biClrUsed;////////////位图实际使用的颜色表中的颜色数，若该值为0,则使用颜色数为2的biBitCount次方
	DWORD biClrImportant;///////位图显示过程中重要的颜色数，若该值为0,则所有的颜色都重要
}BITMAPINFOHEADER;
#pragma pack()//////////////////取消自定义字节方式
/*********** *********** *********** *********** *********** *********** ***********
* definition :struct
* Description :调色板
*********** *********** *********** *********** *********** *********** ***********/
typedef struct tagRGBQUAD
{
	BYTE rgbBlue;///////////////蓝色的亮度（0-255)
	BYTE rgbGreen;//////////////绿色的亮度（0-255)
	BYTE rgbRed;////////////////红色的亮度（0-255)
	BYTE rgbReserved;///////////保留，必须为0
}RGBQUAD;


bool ReadBmp(BITMAPFILEHEADER &fileHeader, BITMAPINFOHEADER &infoHeader, RGBQUAD *&rgbPalette, void *img[500], char *FileName)
{
	FILE *fpIn = fopen(FileName, "rb");
	int i = 0, sizeOfHang = 0, sizeOfPalette = 0;

	if (fpIn == NULL)
		return false;

	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpIn);
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpIn);

	if (infoHeader.biBitCount<16)
	{
		sizeOfPalette = int(pow(2.0, infoHeader.biBitCount));
		rgbPalette = new RGBQUAD[sizeOfPalette];

		fread(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpIn);
	}


	if ((infoHeader.biBitCount * infoHeader.biWidth) % 32 == 0)
		sizeOfHang = (infoHeader.biBitCount * infoHeader.biWidth) / 8;
	else
		sizeOfHang = ((infoHeader.biBitCount * infoHeader.biWidth) / 32 + 1) * 4;

	for (i = 0; i<infoHeader.biHeight; i++)
	{
		img[i] = (void *) new BYTE[sizeOfHang];
		fread(img[i], sizeOfHang, 1, fpIn);
	}

	fclose(fpIn);
	return true;
}


void SaveBmp(BITMAPFILEHEADER &fileHeader, BITMAPINFOHEADER &infoHeader, RGBQUAD *rgbPalette, void *img[500], char *FileName)
{
	FILE *fpOut = fopen(FileName, "wb");
	int i = 0, sizeOfHang = 0, sizeOfPalette = 0;

	fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpOut);
	fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpOut);

	if (infoHeader.biBitCount<16)
	{
		sizeOfPalette = int(pow(2.0, infoHeader.biBitCount));
		fwrite(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpOut);
	}


	if ((infoHeader.biBitCount * infoHeader.biWidth) % 32 == 0)
		sizeOfHang = (infoHeader.biBitCount * infoHeader.biWidth) / 8;
	else
		sizeOfHang = ((infoHeader.biBitCount * infoHeader.biWidth) / 32 + 1) * 4;

	for (i = 0; i<infoHeader.biHeight; i++)
		fwrite(img[i], sizeOfHang, 1, fpOut);

	fclose(fpOut);
}

void dft(short in_array[100][100], double re_array[100][100], double im_array[100][100], long height, long width)
{
	double re, im, temp;
	double pi = 3.1415926535898;
	printf("dft start\n");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			re = 0;
			im = 0;

			for (int x = 0; x < height; x++) {
				for (int y = 0; y < width; y++) {
					temp = (double)i * x / (double)height +
						(double)j * y / (double)width;
					re += in_array[x][y] * cos(-2 * pi * temp);
					im += in_array[x][y] * sin(-2 * pi * temp);
				}
			}

			re_array[i][j] = re;
			im_array[i][j] = im;
		}
	}
	printf("dft done\n");
}

void idft(double re_array[100][100], double im_array[100][100], short out_array[100][100], long height, long width)
{
	double real, temp;
	double pi = 3.1415926535898;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			real = 0;

			for (int x = 0; x < height; x++) {
				for (int y = 0; y < width; y++) {
					temp = (double)i * x / (double)height +
						(double)j * y / (double)width;
					real += re_array[x][y] * cos(2 * pi * temp) -
						im_array[x][y] * sin(2 * pi * temp);
				}
			}

			out_array[i][j] = (short)((float)real / sqrt(float(width*height)));
			if (out_array[i][j] > 0xff)
				out_array[i][j] = 0xff;
			else if (out_array[i][j] < 0)
				out_array[i][j] = 0;
		}
	}
	printf("idft done\n");
}

void DFT_Shift(short array[100][100], int height, int width) 
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			if ((i + j) % 2)
			{
				array[j][i] = -array[j][i];
			}
		}
	}
}

void IdealBand(double re_array[100][100], double im_array[100][100], int d0, int w, int size_h, int size_w)
{
	for (int j = 0; j<size_h; j++)
		for (int i = 0; i<size_w; i++)
		{
			double distance = sqrt(double(j - size_h / 2)*(j - size_h / 2) + (i - size_w / 2)*(i - size_w / 2));
			//printf("%f ", distance);
			if (distance<(d0 - w / 2) || distance>(d0 + w / 2))
			{
				if (re_array[j][i] != 0)
					printf("re_array");
				if (im_array[j][i] != 0)
					printf("im_array");
				re_array[j][i] = 0.0;
				im_array[j][i] = 0.0;
			}
		}
}

int main()
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	double re_array[100][100];
	double im_array[100][100];
	short out_array[100][100];
	short array[100][100];
	short array_f[100][100];

	RGBQUAD *rgbPalette;//调色板

	void * img[500];
	char *FileName = "2.bmp";

	ReadBmp(fileHeader, infoHeader, rgbPalette, img, FileName);

	for (int i = 0; i < infoHeader.biHeight; i++)
		for (int j = 0; j < infoHeader.biWidth; j++)
		{
			array[i][j] = (short)((unsigned char *)img[i])[j];
		}
	int width = infoHeader.biWidth;
	int height = infoHeader.biHeight;
	DFT_Shift(array, (int)infoHeader.biHeight, (int)infoHeader.biWidth);
	dft(array, re_array, im_array, (long)infoHeader.biHeight, (long)infoHeader.biWidth);
	IdealBand(re_array, im_array, 8, 8, (int)infoHeader.biHeight, (int)infoHeader.biWidth);
	
	idft(re_array, im_array, out_array, (long)infoHeader.biHeight, (long)infoHeader.biWidth);
	DFT_Shift(out_array, (int)infoHeader.biHeight, (int)infoHeader.biWidth);

	for (int i = 0; i < infoHeader.biHeight; i++)
	{
		for (int j = 0; j < infoHeader.biWidth; j++)
		{
			((unsigned char *)img[i])[j] = (unsigned char)((int)out_array[i][j]);
		}
	}


	SaveBmp(fileHeader, infoHeader, rgbPalette, img, "test_target_ball.bmp");
	system("pause");
	return 0;

}