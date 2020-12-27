#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//using namespace std;
using namespace cv;

int main()
{   
	Mat img = imread("picture.jpg");
	//namedWindow("сно╥");
	imshow("image", img);
	waitKey(0);
	//system("pause");
	return 0;
}