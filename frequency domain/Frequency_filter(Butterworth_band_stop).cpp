#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

cv::Mat image_make_border(cv::Mat &src)
{
	int w = getOptimalDFTSize(src.cols);
	int h = getOptimalDFTSize(src.rows);
	Mat padded;
	copyMakeBorder(src, padded, 0, h - src.rows, 0, w - src.cols, BORDER_CONSTANT, Scalar::all(0));
	padded.convertTo(padded, CV_32FC1);
	return padded;
}

//频率域滤波
Mat frequency_filter(Mat &scr, Mat &blur)
{
	//***********************DFT*******************
	Mat plane[] = { scr, Mat::zeros(scr.size() , CV_32FC1) }; //创建通道，存储dft后的实部与虚部（CV_32F，必须为单通道数）
	Mat complexIm;
	merge(plane, 2, complexIm);//合并通道 （把两个矩阵合并为一个2通道的Mat类容器）
	dft(complexIm, complexIm);//进行傅立叶变换，结果保存在自身

	//***************中心化********************
	split(complexIm, plane);//分离通道（数组分离）
							//    plane[0] = plane[0](Rect(0, 0, plane[0].cols & -2, plane[0].rows & -2));//这里为什么&上-2具体查看opencv文档
							//    //其实是为了把行和列变成偶数 -2的二进制是11111111.......10 最后一位是0
	int cx = plane[0].cols / 2; 
	int cy = plane[0].rows / 2;//以下的操作是移动图像  (零频移到中心)
	Mat part1_r(plane[0], Rect(0, 0, cx, cy));  //元素坐标表示为(cx,cy)
	Mat part2_r(plane[0], Rect(cx, 0, cx, cy));
	Mat part3_r(plane[0], Rect(0, cy, cx, cy));
	Mat part4_r(plane[0], Rect(cx, cy, cx, cy));

	Mat temp;
	part1_r.copyTo(temp);  //左上与右下交换位置(实部)
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp);  //右上与左下交换位置(实部)
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	Mat part1_i(plane[1], Rect(0, 0, cx, cy));  //元素坐标(cx,cy)
	Mat part2_i(plane[1], Rect(cx, 0, cx, cy));
	Mat part3_i(plane[1], Rect(0, cy, cx, cy));
	Mat part4_i(plane[1], Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp);  //左上与右下交换位置(虚部)
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp);  //右上与左下交换位置(虚部)
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);

	//*****************滤波器函数与DFT结果的乘积****************
	Mat blur_r, blur_i, BLUR;
	multiply(plane[0], blur, blur_r); //滤波（实部与滤波器模板对应元素相乘）
	multiply(plane[1], blur, blur_i);//滤波（虚部与滤波器模板对应元素相乘）
	Mat plane1[] = { blur_r, blur_i };
	merge(plane1, 2, BLUR);//实部与虚部合并

	magnitude(plane1[0], plane1[1], plane1[0]);//获取幅度图像，0通道为实部通道，1为虚部，因为二维傅立叶变换结果是复数
	plane1[0] += Scalar::all(1);  //傅立叶变换后的图片不好分析，进行对数处理，结果比较好看
	log(plane1[0], plane1[0]);    // float型的灰度空间为[0，1])
	normalize(plane1[0], plane1[0], 1, 0, CV_MINMAX);  //归一化便于显示
	imshow("滤波后频谱图", plane1[0]);

	//*********************得到原图频谱图***********************************
	magnitude(plane[0], plane[1], plane[0]);//获取幅度图像，0通道为实部通道，1为虚部，因为二维傅立叶变换结果是复数
	plane[0] += Scalar::all(1);  //傅立叶变换后的图片不好分析，进行对数处理，结果比较好看
	log(plane[0], plane[0]);    // float型的灰度空间为[0，1])
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);  //归一化便于显示
	imshow("原图频谱图", plane[0]);

	//*********************图像IDFT***********************************
	idft(BLUR, BLUR);    //idft结果也为复数
	split(BLUR, plane);//分离通道，主要获取通道
	magnitude(plane[0], plane[1], plane[0]);  //求幅值(模)
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);  //归一化便于显示
	return plane[0];//返回参数
}

//*****************巴特沃斯带阻滤波器***********************
Mat butterworth_band_stop_kernel(Mat &scr, float sigma, float w, int n)
{
	Mat butterworth_low_pass(scr.size(), CV_32FC1); //，CV_32FC1
	double D0 = sigma;//半径D0越小，模糊越大；半径D0越大，模糊越小
	double W = w;
	for (int i = 0; i<scr.rows; i++) {
		for (int j = 0; j<scr.cols; j++) {
			double d = sqrt(pow((i - scr.rows / 2), 2) + pow((j - scr.cols / 2), 2));//分子,计算pow必须为float型
			butterworth_low_pass.at<float>(i, j) = 1.0 / (1 + pow((d * W)/(pow(d, 2) - pow(D0, 2)), 2 * n));
		}
	}

	string name = "巴特沃斯带阻滤波器d0=" + std::to_string(sigma) + "n=" + std::to_string(n);
	imshow(name, butterworth_low_pass);
	return butterworth_low_pass;
}

//巴特沃斯带阻滤波器
Mat butterworth_band_stop_filter(Mat &src, float d0, float w, int n)
{
	//H = 1 / (1+(D0/D)^2n)    n表示巴特沃斯滤波器的次数
	Mat padded = image_make_border(src);
	Mat butterworth_kernel = butterworth_band_stop_kernel(padded, d0, w, n);
	Mat result = frequency_filter(padded, butterworth_kernel);
	return result;
}

int main(int argc, char *argv[])
{
	Mat input = imread("bad.jpg", IMREAD_GRAYSCALE);
	if (input.empty())
		return -1;
	imshow("原图", input);//显示原图

	cv::Mat bw_high = butterworth_band_stop_filter(input, 150, 300, 4);
	bw_high = bw_high(cv::Rect(0, 0, input.cols, input.rows));
	imshow("巴特沃斯带阻", bw_high);


	waitKey();
	return 0;
}
