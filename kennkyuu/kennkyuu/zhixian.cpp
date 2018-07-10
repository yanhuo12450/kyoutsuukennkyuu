#include "stdafx.h"
#include "zhixian.h"

Mat test3;
int zhixian() {
	namedWindow("src", WINDOW_AUTOSIZE);
	setMouseCallback("src", on_mouse, 0);
	Mat test, test2;
	String str = "201710191540.png";
	test = imread("data/" + str);//âÊëúÇÃì«Ç›çûÇ›
	test.copyTo(test3);
	if (test.data == NULL) {//âÊëúÇÃì«Ç›çûÇ›Ç™ê≥ÇµÇ¢Ç©Ç«Ç§Ç©ÇîªífÇµÇ‹Ç∑
		cout << "cannot read a picture" << endl;
		return 1;
	}
	imshow("src", test3);
	findtheshiro(test3,test2);
	killnoise(test3, 10);
	vector<vector<double>> tane;
	tane.resize(2);
	tane[0].resize(3);
	tane[1].resize(3);
	tane[0][0] = test3.at<uchar>(test3.rows / 2, test3.cols / 4);
	tane[0][1] = test3.rows / 2;
	tane[0][2] = test3.cols / 4;
	tane[1][0] = test3.at<uchar>(test3.rows / 2, test3.cols / 4*3);
	tane[1][1] = test3.rows / 2;
	tane[1][2] = test3.cols / 4*3;
	Kmeans(test3, tane, 2);

	imshow("src2", test3);
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

void findtheshiro(Mat &src, Mat &dst) {
	for (int i = 0; i < src.rows; i++)
	{
		int avgr = 0, avgg = 0, avgb = 0;
		for (int j = 0; j < src.cols; j++)
		{
			avgb += src.at<Vec3b>(i, j)[0];
			avgg += src.at<Vec3b>(i, j)[1];
			avgr += src.at<Vec3b>(i, j)[2];
		}
		avgb /= src.cols;
		avgg /= src.cols;
		avgr /= src.cols;
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<Vec3b>(i, j)[0] >= avgb)src.at<Vec3b>(i, j)[0] -= avgb; else src.at<Vec3b>(i, j)[0] = 0;
			if(src.at<Vec3b>(i, j)[1]>=avgg)src.at<Vec3b>(i, j)[1]-=avgg; else src.at<Vec3b>(i, j)[1] = 0;
			if(src.at<Vec3b>(i, j)[2]>=avgr)src.at<Vec3b>(i, j)[2]-=avgr; else src.at<Vec3b>(i, j)[2] = 0;
		}
	}
	
}

void killnoise(Mat &src, int windowssize) {
	cvtColor(src,src,CV_BGR2GRAY);
	Mat dst;
	dst = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols-windowssize; j++) {
			int sum = 0;
			for (int k = 0; k < windowssize; k++) {
				sum += src.at<uchar>(i, j+k);
			}
			if (sum > windowssize * 20)dst.at<uchar>(i, j) = 255; else dst.at<uchar>(i, j) = 0;
		}
	}
	src = dst;
}

void tajyuukaisoudo(Mat &src, int tajyuu) {
	Mat dst;
	dst = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows-tajyuu*3; i++) {
		for (int j = 0; j < src.cols-tajyuu*3; j++) {
			int toupiao = 0;
			int sum = 0; 
			for (int n = 0; n < tajyuu; n++) {
				for (int p = 0; p < n * 3; p++) {
					for (int q = 0; q < n * 3; q++) {
						if (src.at<uchar>(i + p, j + q) == 255)sum += 1;
					}
				}
				if (sum > n*n * 3)toupiao += 1;
			}
			if (toupiao > tajyuu / 2)dst.at<uchar>(i, j) = 255;
		}
	}
	src = dst;
}

void Kmeans(Mat &src, vector<vector<double>>p, int k) {
	Mat dst;
	dst = Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			int kyouri = 0, mini = 59999;
			for (int n = 0; n < k; n++) {
				kyouri =
					(src.at<uchar>(i, j) - p[k][0])*(src.at<uchar>(i, j) - p[k][0]) +
					(i - p[k][1])*(i - p[k][1]) + (j - p[k][2])*(j - p[k][2]);
				if (kyouri < mini)
					dst.at<uchar>(i, j) = n;
			}

		}
	vector<vector<double>>q;
	q.resize(k);
	for (int i = 0; i < k; i++) {
		q[i].resize(3);
	}
	vector<int> count;
	count.resize(k);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int s = dst.at<uchar>(i, j);
			q[s][0] += src.at<uchar>(i, j);
			q[s][1] += i;
			q[s][2] += j;
			count[s]++;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < 3; j++) {
			q[i][j] /= count[i];
		}
	}
	double piancha = 0;
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < 3; j++) {
			piancha += (p[i][j] - q[i][j])*(p[i][j] - q[i][j]);
		}
	}
	if (piancha < 1)
		return;
	Kmeans(src, q, k);
	imshow("kekka", dst);
}