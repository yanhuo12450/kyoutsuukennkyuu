#pragma once
#ifndef zhixian_h
#define zhixian_h
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

int zhixian();

void RGBAVG(Mat &src, int step);

void GrayAVG(Mat src, int step);

void ToHSV(Mat src);

void on_mouse(int event, int x, int y, int flags, void *ustc);
void findtheshiro(Mat &src, Mat &dst);
void killnoise(Mat &src, int windowssize);
void tajyuukaisoudo(Mat &src, int tajyuu);
void Kmeans(Mat &src, vector<vector<double>>p, int k);
#endif // !zhixian_h


