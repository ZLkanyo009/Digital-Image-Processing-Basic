#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int Otsu(cv::Mat& src, cv::Mat& dst, int thresh) {
	const int Grayscale = 256;
	int graynum[Grayscale] = { 0 };
	int r = src.rows;
	int c = src.cols;
	for (int i = 0; i < r; ++i) {
		const uchar* ptr = src.ptr<uchar>(i);
		for (int j = 0; j < c; ++j) {        //ֱ��ͼͳ��
			graynum[ptr[j]]++;
		}
	}

	double P[Grayscale] = { 0 };
	double PK[Grayscale] = { 0 };
	double MK[Grayscale] = { 0 };
	double srcpixnum = r*c, sumtmpPK = 0, sumtmpMK = 0;
	for (int i = 0; i < Grayscale; ++i) {
		P[i] = graynum[i] / srcpixnum;   //ÿ���Ҷȼ�ռ�����ظ���֮��(Ҳ�ɳ�Ϊ����)
		PK[i] = sumtmpPK + P[i];         //ռ���ۼƺ� PK��ʾP(0->k)
		sumtmpPK = PK[i];
		MK[i] = sumtmpMK + i*P[i];       //�Ҷȼ����ۼӾ�ֵ    MK��ʾM(0->k)                                                                                                                                                                                                                                                                                                                                                                                                    
		sumtmpMK = MK[i];
	}

	//������䷽��
	double Var = 0;
	for (int k = 0; k < Grayscale; ++k) {
		if ((MK[Grayscale - 1] * PK[k] - MK[k])*(MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k])) > Var) {
			Var = (MK[Grayscale - 1] * PK[k] - MK[k])*(MK[Grayscale - 1] * PK[k] - MK[k]) / (PK[k] * (1 - PK[k]));
			thresh = k;
		}
	}

	//��ֵ����(��ֵ��)
	src.copyTo(dst);
	for (int i = 0; i < r; ++i) {
		uchar* ptr = dst.ptr<uchar>(i);
		for (int j = 0; j < c; ++j) {
			if (ptr[j]> thresh)
				ptr[j] = 255;
			else
				ptr[j] = 0;
		}
	}
	return thresh;
}


int main() {
	cv::Mat src = cv::imread("picture2.jpg");
	if (src.empty()) {
		return -1;
	}
	if (src.channels() > 1)
		cv::cvtColor(src, src, CV_RGB2GRAY);

	cv::Mat dst, dst2;
	int thresh = 0;
	thresh = Otsu(src, dst, thresh); //Otsu
	std::cout << "Mythresh=" << thresh << std::endl;
	double  Otsu = 0;

	Otsu = cv::threshold(src, dst2, Otsu, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	std::cout << "OpenCVthresh=" << Otsu << std::endl;

	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", src);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);
	cv::namedWindow("dst2", CV_WINDOW_NORMAL);
	cv::imshow("dst2", dst2);
	cv::waitKey(0);
}