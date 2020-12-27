#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//int main(void)
//{
//	// [1] src读入图片
//	cv::Mat src = cv::imread("dst2.jpg");
//	cv::Mat src2 = cv::imread("dst3.jpg");
//	// [2] dst目标图片
//	cv::Mat dst;
//	// [3] 滤波 N越大越平越模糊(2*N+1) sigmas空间越大越模糊sigmar相似因子
//	bilateralFilter(src, dst, 10, 50 , 10);
//	//myBialteralFilter(&src, &dst, 25, 12.5, 50);
//	//addWeighted(src, 0.5, src2, 1.5, 0, dst);
//	imwrite("dst4.jpg", dst);
//	// [4] 窗体显示
//	//cv::imshow("src 1006534767", src);
//	//cv::imshow("dst 1006534767", dst);
//	//cv::waitKey(0);
//	//cv::destroyAllWindows();
//
//	return 0;
//}

int main(int artc, char** argv) {
	Mat src = imread("blur.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	//namedWindow("input", WINDOW_AUTOSIZE);
	//imshow("input", src);

	Mat blur_img, usm,susm,ssusm,dst;
	GaussianBlur(src, blur_img, Size(0, 0), 25);
	//blur(blur_img, blur_img, Size(0, 0));
	addWeighted(src, 2.5, blur_img, -1.5, 1, usm);
	//Scale(usm, susm);
	//imshow("mask image", usm);
	//src.convertTo(src, CV_32F, 1.0 );
	//cout << src.depth() << endl;
	//Laplacian(src, usm, CV_32F, 3);
	//addWeighted(src, 1.5, usm, 0.5, 0, dst);
	imwrite("dst2.jpg", usm);



	waitKey(0);
	return 0;
}
