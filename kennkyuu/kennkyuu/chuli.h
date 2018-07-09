#pragma once
#include "opencv2\opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

vector<double> AvgGLCM(Mat &test, int Gray_level);
void TestKmeans(Mat &test, int K);
void XKmeans(vector<vector<vector<double>>> D, int K, vector<vector<double>> Tane, Mat &outputmat);
int chuli();