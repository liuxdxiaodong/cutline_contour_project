#include <iostream>
#include <cstring>
#include <fstream>
#include <math.h>
#include <opencv2\opencv.hpp>

using namespace std;


struct Data
{
	double toolLen;       //棒料长度
	double toolD;         //铣刀直径
	double cutEdgelen;    //切削刃长度
	double taperAgl;      //锥度角度
	double helixAgl;      //螺旋角
	double precision;     //精度
};

struct Calculate
{
	
	double toolR;          // 铣刀半径
	double lead;           // 导程
	int numofpoint;        // 点的个数
};

/* get the Point sets of the cutline projecting to the image*/
void getpoint(Data data,double* arrar,cv::Mat imav, const char* filename);

/* Get the Point sets of the contourline projecting to the image*/
void contourLine(Data data,double* arrar,cv::Mat imav,const char* filename);

/* project vector with Point3d to the 2D image*/
void Proimage(double* arrar,vector<cv::Point3d> vec, cv::Mat imav, const char* filename, int n);
