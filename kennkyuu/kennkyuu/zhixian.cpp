#include "stdafx.h"
#include "zhixian.h"

Mat test3;
int zhixian() {
	namedWindow("src", WINDOW_AUTOSIZE);
	setMouseCallback("src", on_mouse, 0);
	Mat test, test2;
	String str = "201710191540.png";
	test = imread("data/" + str);//‰æ‘œ‚Ì“Ç‚İ‚İ
	test.copyTo(test3);
	if (test.data == NULL) {//‰æ‘œ‚Ì“Ç‚İ‚İ‚ª³‚µ‚¢‚©‚Ç‚¤‚©‚ğ”»’f‚µ‚Ü‚·
		cout << "cannot read a picture" << endl;
		return 1;
	}
	imshow("src", test3);
	RGBAVG(test3, 2);
	waitKey(0);
	return 0;
}

void RGBAVG(Mat &src, int step) {
	//Mat drt = Mat::zeros(src.cols/step, src.rows/step, CV_8UC3);
	Mat drt=Mat::zeros(src.rows / step, src.cols / step, CV_8UC3);
	Mat avgR = Mat::zeros(src.rows / step, src.cols / step, CV_8UC1);
	Mat avgG = Mat::zeros(src.rows / step, src.cols / step, CV_8UC1);
	Mat avgB = Mat::zeros(src.rows / step, src.cols / step, CV_8UC1);
	
	int sosumb = 0, sosumg = 0, sosumr = 0;
	for (int i = 0; i < src.cols/step; i++)
	{	
		for (int j = 0; j < src.rows/step ; j++) {
			int sumr = 0;
			int sumg = 0;
			int sumb = 0;
			for (int k = 0; k < step; k++) {
				for (int y = 0; y < step; y++) {
					sumb += src.at<Vec3b>(j * step + y, i * step + k)[0];
					sumg += src.at<Vec3b>(j * step + y, i * step + k)[1];
					sumr += src.at<Vec3b>(j * step + y, i * step + k)[2];
				}
			}
			sosumb += sumb;
			sosumg += sumg;
			sosumr += sumr;
			drt.at<Vec3b>(j,i)[0] = (uchar)sumb / (step*step);
			drt.at<Vec3b>(j,i)[1] = (uchar)sumg / (step*step);
			drt.at<Vec3b>(j,i)[2] = (uchar)sumr / (step*step);
			avgR.at<uchar>(j, i) = (uchar)sumr / (step*step);
			avgG.at<uchar>(j, i) = (uchar)sumg / (step*step);
			avgB.at<uchar>(j, i) = (uchar)sumb / (step*step);
		}
	}
	sosumb /= (src.rows*src.cols);
	sosumg /= (src.rows*src.cols);
	sosumr /= (src.rows*src.cols);
	cout << "sosumb" << sosumb << "sosumg" << sosumg << "sosumr" << sosumr << endl;

	imshow("rgbavg", drt);
	imshow("r", avgR);
	imshow("g", avgG);
	imshow("b", avgB);
}


void GrayAVG(Mat src, int step) {
	cvtColor(src, src, CV_BGR2GRAY);
	Mat dst = Mat::zeros(src.rows / step, src.cols / step, CV_8UC1);
	for (int i = 0; i < src.rows / step; i++)
		for (int j = 0; j < src.cols / step; j++) {
			int grayavg = 0;
			for (int k = 0; k < step; k++) {
				for (int y = 0; y < step; y++) {
					grayavg += src.at<uchar>(i*step + k, j*step + y);
				}
			}
			dst.at<uchar>(i, j) = grayavg / (step*step);
		}

	imshow("grayavg", dst);
}

void ToHSV(Mat src) {
	cvtColor(src, src, CV_BGR2HSV);
	imshow("hsv", src);
}

void on_mouse(int event, int x, int y, int flags, void *ustc) {
	
	if (event == CV_EVENT_LBUTTONDOWN) {
		cout << "x=" << x << "y=" << y <<"B= "<< (int)test3.at<Vec3b>(y, x)[0] << "G= " << (int)test3.at<Vec3b>(y, x)[1] << "R= " << (int)test3.at<Vec3b>(y, x)[2] << "avg= " << (int)(test3.at<Vec3b>(y, x)[0]+ test3.at<Vec3b>(y, x)[1]+ test3.at<Vec3b>(y, x)[2])/3 << endl;
	}


}