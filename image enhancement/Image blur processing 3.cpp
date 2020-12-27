#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("clear.jpg"), dst;

	blur(src, dst, Size(80, 80));


	imshow("src", src);
	imshow("dst", dst);
	imwrite("dst.jpg", dst);
	waitKey();
	return 0;
}