#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("clear.jpg"), dst;
	
	resize(src, dst, Size(96, 96));
	resize(dst, dst, Size(src.cols , src.rows ));
	
	imshow("src", src);
	imshow("dst", dst);
	imwrite("dst_resize.jpg",dst);
	waitKey();
	return 0;
}