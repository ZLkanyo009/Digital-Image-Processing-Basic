
#include<opencv2/opencv.hpp>
#include<iostream>
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

void main()
{
	Mat srcImage = imread("模糊.jpg");
	if (!srcImage.data) {
		cout << "falied to read" << endl;
		system("pause");
		return;
	}
	Mat srcGray;
	cvtColor(srcImage, srcGray, CV_BGR2GRAY);
	//高斯滤波
	GaussianBlur(srcGray, srcGray, Size(3, 3),
		0, 0, BORDER_DEFAULT);
	//Canny检测
	int edgeThresh = 100;
	Mat Canny_result;
	Canny(srcImage, Canny_result, edgeThresh, edgeThresh * 3, 3);
	cvtColor(Canny_result, Canny_result, CV_GRAY2RGB);
	Canny_result = Canny_result * 2 + srcImage;
	//imshow("src", srcImage);
	imwrite("Canny_result.jpg", Canny_result);
	waitKey(0);
}