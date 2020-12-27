
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
void main()
{
	Mat srcImage = imread("blur.jpg");
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
	//拉普拉斯变换
	Mat laplace_result;
	Laplacian(srcGray, laplace_result, CV_16S, 3);
	convertScaleAbs(laplace_result, laplace_result);
	imshow("src", srcImage);
	imshow("Laplace_result", laplace_result);
	addWeighted(laplace_result, 0.5, srcImage, 0.5, 0, laplace_result);
	imwrite("Laplace_result.jpg", laplace_result);
	waitKey(0);
}