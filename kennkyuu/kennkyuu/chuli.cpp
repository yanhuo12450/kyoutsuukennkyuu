#include "stdafx.h"
#include "chuli.h"

int chuli() {
	Mat test, test2;
	String str = "201710191540.png";
	test = imread("data/" + str);//�摜�̓ǂݍ���
	test.copyTo(test2);
	if (test.data == NULL) {//�摜�̓ǂݍ��݂����������ǂ����𔻒f���܂�
		cout << "cannot read a picture" << endl;
		return 1;
	}
	TestKmeans(test, 2);//�摜�̃Z�O�����e�[�V�����A���S���Y��
	imshow("asdf", test2);
	for (int i = 0; i < test.rows - 1; i++) {
		for (int n = 0; n < test.cols - 1; n++) {
			if (test.at<uchar>(i, n) != test.at<uchar>(i + 1, n) || test.at<uchar>(i, n) != test.at<uchar>(i, n + 1)) {
				test2.at<Vec3b>(i, n)[0] = 255;
				test2.at<Vec3b>(i, n)[1] = 255;
				test2.at<Vec3b>(i, n)[2] = 255;
			}
		}
	}
	imshow("����", test2);
	imwrite("kekka/" + str, test2);
	waitKey(0);
	return 0;
}



vector<double> AvgGLCM(Mat &test, int Gray_level) {//�摜��GLCM�s����v�Z���A�����ʂ̃x�N�g����Ԃ��܂�
	if (test.channels() != 1) {//�摜�̓O���[���ǂ����𔻒f���܂�
		cout << "pic is not gray" << endl;
		vector<double> aa;
		return aa;
	}
	//�l������GLCM���v�Z���܂�
	Mat temp0 = Mat::zeros(Gray_level, Gray_level, CV_8UC1);
	Mat temp45 = Mat::zeros(Gray_level, Gray_level, CV_8UC1);
	Mat temp90 = Mat::zeros(Gray_level, Gray_level, CV_8UC1);
	Mat temp135 = Mat::zeros(Gray_level, Gray_level, CV_8UC1);
	double energy = 0, entropy = 0, contrast = 0, idMoment = 0, ENE = 0, ENT = 0, CON = 0, COR = 0, ui = 0, uj = 0, si_2 = 0, sj_2 = 0;//GLCM�̓����ʂ��v�Z���܂�
	int sum = 0;
	vector<double> Di;//GLCM�̓����ʂ̃x�N�g��
	Di.resize(4);

	for (int i = 0; i < test.rows; i++) {//0�x���̃O���[�X�P�[���������N�s��
		for (int n = 0; n < test.cols - 1; n++) {
			int x = test.at<uchar>(i, n);
			int y = test.at<uchar>(i, n + 1);
			temp0.at<uchar>(x, y)++;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			sum += temp0.at<uchar>(i, n);
		}
	}
	Mat temp = Mat::zeros(Gray_level, Gray_level, CV_32FC1);
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level - 1; n++) {
			temp.at<float>(i, n) = (double)temp0.at<uchar>(i, n) / sum;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			double p = temp.at<float>(i, n);
			if (p > 0)
				ENT += -p * log(p);
			ENE += p * p;
			CON += (i - n)*(i - n)*p;
			ui += i * p;
			uj += n * p;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			si_2 += temp.at<float>(i, n)*(i - ui)*(i - ui);
			sj_2 += temp.at<float>(i, n)*(n - uj)*(n - uj);
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			COR += temp.at<float>(i, n)*(i - ui)*(n - uj) / (sqrt(si_2)*sqrt(sj_2));
		}
	}
	//cout << " ENT " << ENT << " CON " << CON << " COR " << COR << endl;
	Di[0] += ENT;
	Di[1] += CON;
	if (COR>0)
		Di[2] += COR;
	Di[3] += ENE;


	energy = 0, entropy = 0, contrast = 0, idMoment = 0, sum = 0, ENE = 0, ENT = 0, CON = 0, COR = 0, ui = 0, uj = 0, si_2 = 0, sj_2 = 0;
	temp = Mat::zeros(Gray_level, Gray_level, CV_32FC1);
	for (int i = 0; i < test.rows - 1; i++) {//45�x���̃O���[�X�P�[���������N�s��
		for (int n = 0; n < test.cols - 1; n++) {
			temp45.at<uchar>(test.at<uchar>(i, n), test.at<uchar>(i + 1, n + 1))++;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			sum += temp45.at<uchar>(i, n);
		}
	}

	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			temp.at<float>(i, n) = (double)temp45.at<uchar>(i, n) / sum;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level - 1; n++) {
			double p = temp.at<float>(i, n);
			if (p>0)
				ENT += -p * log(p);
			ENE += p * p;
			CON += (i - n)*(i - n)*p;
			ui += i * p;
			uj += n * p;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			si_2 += temp.at<float>(i, n)*(i - ui)*(i - ui);
			sj_2 += temp.at<float>(i, n)*(n - uj)*(n - uj);
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			COR += temp.at<float>(i, n)*(i - ui)*(n - uj) / (sqrt(si_2)*sqrt(sj_2));
		}
	}
	//cout << " ENT " << ENT << " CON " << CON << " COR " << COR << endl;
	Di[0] += ENT;
	Di[1] += CON;
	if (COR>0)
		Di[2] += COR;
	Di[3] += ENE;




	energy = 0, entropy = 0, contrast = 0, idMoment = 0, sum = 0, ENE = 0, ENT = 0, CON = 0, COR = 0, ui = 0, uj = 0, si_2 = 0, sj_2 = 0;
	temp = Mat::zeros(Gray_level, Gray_level, CV_32FC1);
	for (int i = 0; i < test.rows - 1; i++) {//90�x���̃O���[�X�P�[���������N�s��
		for (int n = 0; n < test.cols; n++) {
			temp90.at<uchar>(test.at<uchar>(i, n), test.at<uchar>(i + 1, n))++;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			sum += temp90.at<uchar>(i, n);
		}
	}

	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			temp.at<float>(i, n) = (double)temp90.at<uchar>(i, n) / sum;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level - 1; n++) {
			double p = temp.at<float>(i, n);
			if (p>0)
				ENT += -p * log(p);
			ENE += p * p;
			CON += (i - n)*(i - n)*p;
			ui += i * p;
			uj += n * p;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			si_2 += temp.at<float>(i, n)*(i - ui)*(i - ui);
			sj_2 += temp.at<float>(i, n)*(n - uj)*(n - uj);
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			COR += temp.at<float>(i, n)*(i - ui)*(n - uj) / (sqrt(si_2)*sqrt(sj_2));
		}
	}
	//cout << " ENT " << ENT << " CON " << CON << " COR " << COR << endl;
	Di[0] += ENT;
	Di[1] += CON;
	if (COR>0)
		Di[2] += COR;
	Di[3] += ENE;



	energy = 0, entropy = 0, contrast = 0, idMoment = 0, sum = 0, ENE = 0, ENT = 0, CON = 0, COR = 0, ui = 0, uj = 0, si_2 = 0, sj_2 = 0;
	temp = Mat::zeros(Gray_level, Gray_level, CV_32FC1);
	for (int i = 1; i < test.rows; i++) {//135�x���̃O���[�X�P�[���������N�s��
		for (int n = 1; n < test.cols; n++) {
			temp135.at<uchar>(test.at<uchar>(i, n), test.at<uchar>(i - 1, n - 1))++;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			sum += temp135.at<uchar>(i, n);
		}
	}

	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			temp.at<float>(i, n) = (double)temp135.at<uchar>(i, n) / sum;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level - 1; n++) {
			double p = temp.at<float>(i, n);
			if (p>0)
				ENT += -p * log(p);
			ENE += p * p;
			CON += (i - n)*(i - n)*p;
			ui += i * p;
			uj += n * p;
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			si_2 += temp.at<float>(i, n)*(i - ui)*(i - ui);
			sj_2 += temp.at<float>(i, n)*(n - uj)*(n - uj);
		}
	}
	for (int i = 0; i < Gray_level; i++) {
		for (int n = 0; n < Gray_level; n++) {
			COR += temp.at<float>(i, n)*(i - ui)*(n - uj) / (sqrt(si_2)*sqrt(sj_2));
		}
	}
	//cout << " ENT " << ENT << " CON " << CON << " COR " << COR << endl;
	Di[0] += ENT;
	Di[1] += CON;
	if (COR>0)
		Di[2] += COR;
	Di[3] += ENE;

	return Di;
}

void TestKmeans(Mat &test, int K) {
	Mat test2;
	ofstream ofile;
	ofile.open("GLCM.txt");//GLCM�̍s���ǂݍ���
	if (!ofile.is_open()) {
		cout << "cannot write a file" << endl;
		return;
	}
	int windowsize = 32, step = 5, jigen = 4, gray_level = 8;//�A���S���Y���̃p�����[�^��ݒ肵�܂�
	test.copyTo(test2);
	cvtColor(test2, test2, CV_BGR2GRAY);
	//imwrite("C:/gray2222.jpg", test2);
	equalizeHist(test2, test2);
	//imwrite("C:/hist222.jpg", test2);
	for (int i = 0; i < test2.rows; i++) {
		for (int n = 0; n < test2.cols; n++) {
			test2.at<uchar>(i, n) = test2.at<uchar>(i, n) / (256 / gray_level);//�摜�̃O���[�X�P�[�����x���𒲐����܂�
		}
	}
	//imwrite("C:/8888.jpg", test2);
	vector<vector<vector<double>>> Di;
	Di.resize((test2.rows - windowsize) / step);//�x�N�g���̃T�C�Y�𒲐����܂�
	for (int i = 0; i < (test2.rows - windowsize) / step; i++) {
		Di[i].resize((test2.cols - windowsize) / step);
	}
	for (int i = 0; i < (test2.rows - windowsize) / step; i++) {
		for (int n = 0; n < (test2.cols - windowsize) / step; n++)
			Di[i][n].resize(jigen);
	}
	int o = 0;
	for (int i = 0; i < (test2.rows - windowsize) / step; i++) {
		for (int n = 0; n < (test2.cols - windowsize) / step; n++) {
			Di[i][n] = AvgGLCM(test2(Rect(Point(n*step, i*step), Point(n*step + windowsize, i*step + windowsize))), gray_level);//�摜�̓����ʃx�N�g������������
																																//for (int j = 0; j < jigen; j++) {
																																//	
																																//}
																																//ofile <<"   ";
																																//o++;


		}
		//ofile << endl;
	}
	//cout << (test2.rows - windowsize) / step << "  " << (test2.cols - windowsize) / step << endl;
	//cout << Di.size() <<" "<< Di[0].size() << " "<<Di[0][0].size() << endl;
	for (int j = 0; j < jigen; j++) {
		float min = 100.0, max = 0.0;
		for (int i = 0; i < Di.size(); i++) {
			for (int n = 0; n < Di[i].size(); n++) {
				if (Di[i][n][j] < min)min = Di[i][n][j];
				if (Di[i][n][j] > max)max = Di[i][n][j];
			}
		}
		if (max != min) {
			for (int i = 0; i < Di.size(); i++) {
				for (int n = 0; n < Di[i].size(); n++) {
					Di[i][n][j] = (Di[i][n][j] - min) / (max - min);
					ofile << Di[i][n][j] << "	";
				}

			}
			ofile << endl;
		}

		else {
			ofile.close();
			return;
		}
	}
	ofile.close();


	Mat dist = Mat::zeros(Di.size(), Di[0].size(), CV_8UC1);
	vector<vector<double>> Tane;
	//Kmeans�̎�q�_���v�Z���܂�
	Tane.resize(K);
	for (int i = 0; i < K; i++) {
		Tane[i].resize(jigen + 2);
	}
	for (int i = 0; i < K; i++) {
		Tane[i][0] = i * Di.size() / (float)K;
		Tane[i][1] = i * Di[0].size() / (float)K;
		Tane[i][2] = Di[i*Di.size() / (float)K][i*Di[0].size() / (float)K][0];
		Tane[i][3] = Di[i*Di.size() / (float)K][i*Di[0].size() / (float)K][1];
		Tane[i][4] = Di[i*Di.size() / (float)K][i*Di[0].size() / (float)K][2];
		Tane[i][5] = Di[i*Di.size() / (float)K][i*Di[0].size() / (float)K][3];
		//cout << Tane[i][0] << " " << Tane[i][1] << " " << Tane[i][2] << " " << Tane[i][3] << " " << Tane[i][4] << endl;
	}
	XKmeans(Di, K, Tane, dist);//Kmeans���s���܂�

	equalizeHist(dist, dist);//���ϒl���v�Z���܂�
							 //for (int i = 0;i< (test.rows - windowsize) / step; i++) {
							 //	for (int n = 0; n < (test.cols - windowsize) / step; n++) {
							 //		for (int j = 0; j < step; j++) {
							 //			for (int l = 0; l < step; l++) {
							 //				if (test.channels() == 1) {
							 //					test.at<uchar>(i*step + j, n*step + j) += dist.at<uchar>(i, n);
							 //				}
							 //				if (test.channels() == 3) {
							 //					test.at<Vec3b>(i*step + j, n*step + l)[0] += dist.at<uchar>(i, n);
							 //					test.at<Vec3b>(i*step + j, n*step + l)[1] += dist.at<uchar>(i, n);
							 //					test.at<Vec3b>(i*step + j, n*step + l)[2] += dist.at<uchar>(i, n);
							 //				}
							 //			}
							 //		}
							 //	}
							 //}
	resize(dist, dist, test.size());
	//imshow("dist",dist);
	//imshow("kekka", test);
	test = dist;//�o�������ʂ�Ԃ��܂�
}

void XKmeans(vector<vector<vector<double>>> D, int K, vector<vector<double>> Tane, Mat &outputmat) {
	for (int i = 0; i < D.size(); i++) {//�_�Ƃ̋�������ԏ�����Tane��T���܂�
		for (int n = 0; n < D[i].size(); n++) {
			;
			double maxlen = 600000.0f, sum = 0.0f;
			for (int j = 0; j < K; j++) {//��K��Tane�Ƃ̋������v�Z���܂�
				sum = 0.0f;
				sum += ((float)i - Tane[j][0])*((float)i - Tane[j][0]) / (float)D.size() / (float)D.size();
				sum += ((float)n - Tane[j][1])*((float)n - Tane[j][1]) / (float)D[i].size() / (float)D[i].size();
				sum += (D[i][n][0] - Tane[j][2])*(D[i][n][0] - Tane[j][2]);
				sum += (D[i][n][1] - Tane[j][3])*(D[i][n][1] - Tane[j][3]);
				sum += (D[i][n][2] - Tane[j][4])*(D[i][n][2] - Tane[j][4]);
				sum += (D[i][n][3] - Tane[j][5])*(D[i][n][3] - Tane[j][5]);
				if (sum < maxlen) {
					outputmat.at<uchar>(i, n) = j;//��ԋ߂�Tane�̔ԍ����L�^���܂�
					maxlen = sum;
				}
			}
		}
	}
	for (int i = 0; i < outputmat.rows; i++) {//�L�^�̃}�b�v��\�����܂�
		for (int n = 0; n < outputmat.cols; n++) {
			cout << (int)outputmat.at<uchar>(i, n);
		}
		cout << endl;
	}
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	//Tane�̒l���X�V���܂�
	vector<vector<double>>  Tane2;
	Tane2.resize(K);
	vector<int> Knum;
	Knum.resize(K);
	for (int i = 0; i < K; i++) {
		Tane2[i].resize(6);
	}
	for (int i = 0; i < outputmat.rows; i++) {
		for (int n = 0; n < outputmat.cols; n++) {
			Tane2[outputmat.at<uchar>(i, n)][0] += i;
			Tane2[outputmat.at<uchar>(i, n)][1] += n;
			Tane2[outputmat.at<uchar>(i, n)][2] += D[i][n][0];
			Tane2[outputmat.at<uchar>(i, n)][3] += D[i][n][1];
			Tane2[outputmat.at<uchar>(i, n)][4] += D[i][n][2];
			Tane2[outputmat.at<uchar>(i, n)][5] += D[i][n][3];
			Knum[outputmat.at<uchar>(i, n)]++;
		}
	}
	for (int i = 0; i < K; i++) {
		for (int n = 0; n < 6; n++) {
			Tane2[i][n] /= Knum[i];
		}
	}
	double sum = 0.0f;
	for (int i = 0; i < K; i++) {
		for (int n = 0; n < 6; n++) {
			sum += (Tane2[i][n] - Tane[i][n])*(Tane2[i][n] - Tane[i][n]);
		}
	}
	if (sum>1)
		XKmeans(D, K, Tane2, outputmat);//�J��Ԃ��܂��@
	else return;
}


