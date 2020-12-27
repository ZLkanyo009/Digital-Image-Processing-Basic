#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;


int main()
{
	Mat img;
	img = imread("picture.jpg");

	imshow("image", img);

	/********************* /
	/     1 多通道拆分     /
	/**********************/
	vector<Mat> img_channels;
	//vector<Mat>:将Mat类型用数组形式表示
	split(img, img_channels);
	//split:多通道拆分为单通道

	/********************* /
	/     2 直方图计算     /
	/**********************/
	int histo_size = 256;
	float range[] = { 0,256 };
	const float *histoRanges = { range };
	Mat blue_histo, green_histo, red_histo;
	calcHist(&img_channels[0], 1, 0, Mat(), blue_histo, 1, &histo_size, &histoRanges, true, false);
	calcHist(&img_channels[1], 1, 0, Mat(), green_histo, 1, &histo_size, &histoRanges, true, false);
	calcHist(&img_channels[2], 1, 0, Mat(), red_histo, 1, &histo_size, &histoRanges, true, false);
	//calcHist函数:计算直方图


	/********************* /
	/     3 直方图归一化   /
	/**********************/
	int histo_h = 400;//直方图的图像的高
	int histo_w = 512; //直方图的图像的宽
	int bin_w = histo_w / histo_size;//直方图的刻度等级
	Mat histoImage(histo_w, histo_h, CV_8UC3, Scalar(0, 0, 0));//绘制直方图显示的图像
	normalize(blue_histo, blue_histo, 0, histo_h, NORM_MINMAX, -1, Mat());//归一化
	normalize(green_histo, green_histo, 0, histo_h, NORM_MINMAX, -1, Mat());
	normalize(red_histo, red_histo, 0, histo_h, NORM_MINMAX, -1, Mat());

	/********************* /
	/     4 绘制直方图     /
	/**********************/
	for (int i = 1; i < histo_size; i++)
	{
		//Blue直方图
		line(histoImage, Point((i - 1)*bin_w, histo_h - cvRound(blue_histo.at<float>(i - 1))),
			Point((i)*bin_w, histo_h - cvRound(blue_histo.at<float>(i))), Scalar(255, 0, 0), 2, CV_AA);
		//Green直方图
		line(histoImage, Point((i - 1)*bin_w, histo_h - cvRound(green_histo.at<float>(i - 1))),
			Point((i)*bin_w, histo_h - cvRound(green_histo.at<float>(i))), Scalar(0, 255, 0), 2, CV_AA);
		//Red分量直方图
		line(histoImage, Point((i - 1)*bin_w, histo_h - cvRound(red_histo.at<float>(i - 1))),
			Point((i)*bin_w, histo_h - cvRound(red_histo.at<float>(i))), Scalar(0, 0, 255), 2, CV_AA);
	}
	imshow("histogram_iamge", histoImage);
	waitKey(0);
	return 0;
}