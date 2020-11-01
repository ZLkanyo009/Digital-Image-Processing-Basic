#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	//加载图片
	Mat img, reverse_img;
	img = imread("picture2.jpg");
	imshow("image", img);
	//多通道取反
	reverse_img.create(img.size(), img.type());

	for (int row = 0; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			int B = img.at<Vec3b>(row, col)[0];
			int G = img.at<Vec3b>(row, col)[1];
			int R = img.at<Vec3b>(row, col)[2];
			reverse_img.at<Vec3b>(row, col)[0] = 255 - B;
			reverse_img.at<Vec3b>(row, col)[1] = 255 - G;
			reverse_img.at<Vec3b>(row, col)[2] = 255 - R;
		};

	};
	imshow("reverse_image", reverse_img);
	waitKey(0);
	return 0;
};