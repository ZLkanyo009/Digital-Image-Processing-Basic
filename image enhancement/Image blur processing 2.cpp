#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("ÇåÎú.jpg"), dst;

	GaussianBlur(src, dst, Size(0, 0), 25);

	imshow("src", src);
	imshow("dst", dst);
	imwrite("dst.jpg", dst);
	waitKey();
	return 0;
}