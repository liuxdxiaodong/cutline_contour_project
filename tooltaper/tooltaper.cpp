#include <vector>
#include "common.h"
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(Data data,double* arrar,cv::Mat imav, const char* filename)
{

	double toolR = data.toolD/2;						 //计算过程中所需的参数，toolR:铣刀半径， lead:导程
	double lead = 2*Pi*toolR/tan(data.helixAgl);

	int numofpoint;										 //计算基准线是选取的节点个数
	double a = data.cutEdgelen/data.precision;
	numofpoint = floor(a);
	int n = numofpoint;

	vector<double> xx(n+1);								 //n+1个节点的x坐标
	vector<double> mtoolR(n+1);							 //n+1个节点处对应的铣刀半径，对于锥形铣刀，半径改变
	vector<double> mc(n+1);								 //n+1个节点的旋转角度				 
	vector<double> zz(n+1);								 //n+1个节点的y坐标  
	vector<double> yy(n+1);								 //n+1个节点的z坐标  
	vector<cv::Point3d> vec(n+1);

	for(int i=0;i<n+1;i++)
	{
		xx[i] = 0-(double)i*data.precision;
		mtoolR[i] = -xx[i]*tan(data.taperAgl*Pi/360);
		mc[i] = -2*Pi*xx[i] / lead;
		zz[i] = (toolR+mtoolR[i])*cos(mc[i]);
		yy[i] = (toolR+mtoolR[i])*sin(mc[i]);
		vec[i] = cv::Point3d(xx[i],yy[i],zz[i]);
		
	}
    Proimage(arrar, vec, imav, filename, n);
}

void contourLine(Data data,double* arrar,Mat imav,const char* filename)
{
	double toolR = data.toolD/2;

	int numofpoint;
	double a = data.cutEdgelen/data.precision;
	numofpoint = floor(a);
	int n = numofpoint;

	vector<double> contourX(n+1);
	vector<double> contourY(n+1);
	vector<double> contourZ(n+1);
    vector<Point3d> Cvec(n+1);
	for(int i=0;i<n+1;i++)
	{
		contourX[i] = -i * data.precision;
		contourY[i] = toolR;
		contourZ[i] = 0;
		Cvec[i] = Point3d(contourX[i],contourY[i],contourZ[i]);
	}

	Proimage(arrar, Cvec, imav, filename, n);
	
}

void Proimage(double* arrar,vector<cv::Point3d> vec, Mat imav, const char* filename, int n)
{
	cv::Mat projectB = cv::Mat(3,3, CV_64F,arrar);          //生成一个投影矩阵，将三维点投影到二维平面上

	Mat PointMat = Mat(vec).reshape(1).t();                 //投影矩阵与三维点阵乘积
	cv::Mat result;
	result = projectB * PointMat;

	Mat G = result.row(0).clone();                          //生成图像上的二维点阵
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();

	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	Mat delimav;                                            //去掉负数项,得到最终投影后的有效点
	int t = n+1,count =0;
	for(int i=0;i<t;i++)
		if(imav.at<double>(i,0) < 0 || imav.at<double>(i,1) < 0
			||imav.at<double>(i,0) >1 || imav.at<double>(i,1) >1)
		{
			Mat front = imav(Range(0,i),Range::all());
			Mat back = imav(Range(i+1,t),Range::all());
			vconcat(front,back,delimav);

			imav.release();
			imav = delimav.clone();
			//--------------------//
			i--;
			t--;
		}                                                     // imav为投影后的坐标
	
	ofstream fout(filename, ios_base::out | ios_base::trunc);

	fout << imav << endl;

	fout.close();
}

