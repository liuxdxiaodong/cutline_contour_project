#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace std;


struct Data
{
	double toolLen;       //���ϳ���
	double toolD;         //ϳ��ֱ��
	double cutEdgelen;    //�����г���
	double taperAgl;      //׶�ȽǶ�
	double helixAgl;      //������
	double precision;     //����
};

struct Calculate
{
	
	double toolR;          // ϳ���뾶
	double lead;           // ����
	int numofpoint;        // ��ĸ���
};

/* get the Point sets of the cutline projecting to the image*/
void getpoint(Data data,double* arrar,cv::Mat imav, const char* filename);

/* Get the Point sets of the contourline projecting to the image*/
void contourLine(Data data,double* arrar,cv::Mat imav,const char* filename);

/* project vector with Point3d to the 2D image*/
void Proimage(double* arrar,vector<cv::Point3d> vec, cv::Mat imav, const char* filename, int n);
