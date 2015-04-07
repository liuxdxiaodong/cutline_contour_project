#include <vector>
#include "common.h"
#include "tooltaper.h"
#define Pi 3.1415926

using namespace std;
using namespace cv;


void getpoint(Data data,double* arrar,cv::Mat imav, const char* filename)
{

	double toolR = data.toolD/2;						 //�������������Ĳ�����toolR:ϳ���뾶�� lead:����
	double lead = 2*Pi*toolR/tan(data.helixAgl);

	int numofpoint;										 //�����׼����ѡȡ�Ľڵ����
	double a = data.cutEdgelen/data.precision;
	numofpoint = floor(a);
	int n = numofpoint;

	vector<double> xx(n+1);								 //n+1���ڵ��x����
	vector<double> mtoolR(n+1);							 //n+1���ڵ㴦��Ӧ��ϳ���뾶������׶��ϳ�����뾶�ı�
	vector<double> mc(n+1);								 //n+1���ڵ����ת�Ƕ�				 
	vector<double> zz(n+1);								 //n+1���ڵ��y����  
	vector<double> yy(n+1);								 //n+1���ڵ��z����  
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
	cv::Mat projectB = cv::Mat(3,3, CV_64F,arrar);          //����һ��ͶӰ���󣬽���ά��ͶӰ����άƽ����

	Mat PointMat = Mat(vec).reshape(1).t();                 //ͶӰ��������ά����˻�
	cv::Mat result;
	result = projectB * PointMat;

	Mat G = result.row(0).clone();                          //����ͼ���ϵĶ�ά����
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();

	cv::vconcat(G.mul(1/J),H.mul(1/J),imav);
	imav = imav.t();

	Mat delimav;                                            //ȥ��������,�õ�����ͶӰ�����Ч��
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
		}                                                     // imavΪͶӰ�������
	
	ofstream fout(filename, ios_base::out | ios_base::trunc);

	fout << imav << endl;

	fout.close();
}

