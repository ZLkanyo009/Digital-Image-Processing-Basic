#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void Scale(Mat&input, Mat&output, int index = 0)
{
	float max = 0, min = 0;
	output = Mat::zeros(input.rows, input.cols, CV_8UC3);
	for (int i = 0; i<input.rows; i++)
	{
		float *ptr = input.ptr<float>(i);
		for (int j = 0; j<input.cols * 3; j++)//图像为3通道
		{
			if (max<ptr[j])max = ptr[j];
			if (min>ptr[j])min = ptr[j];
		}

	}//取出图像中的上下限
	for (int i = 0; i<input.rows; i++)
	{
		float *ptr = input.ptr<float>(i);
		uchar *optr = output.ptr<uchar>(i);
		for (int j = 0; j<input.cols * 3; j++)
		{
			if (index)
			{
				if (ptr[j]>1) { optr[j] = 255; continue; }
				else if (ptr[j]<0) { optr[j] = 0; continue; }
				else optr[j] = (uchar)(ptr[j] * 255);
			}
			else
				optr[j] = (uchar)((ptr[j] - min) / (max - min) * 255);
		}
	}
}


int main()
{
	Mat src = imread("medical_bone.png"), gblur, mask, smask, dst;
	Mat laplas;
	Mat output;
	src.convertTo(src, CV_32F, 1.0 / 255, 0);
	GaussianBlur(src, gblur, Size(5, 5), 11, 11);//高斯滤波
	mask = src - gblur;
	Scale(mask, smask);
	dst = mask * 2 + src;
	imshow("src", src);
	imshow("高斯滤波", gblur);
	imshow("mask",3 * mask);
	imshow("scaled_mask", smask);
	imshow("dst", dst);
	waitKey();
	return 0;
}