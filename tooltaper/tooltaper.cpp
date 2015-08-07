#include "common.h"
#include "tooltaper.h"
#include <iomanip>

using namespace std;
using namespace cv;


void getPoint(
	Parameter cutterPara,
	double* arrar,
	cv::Mat imageVector,
	const char* filename
	)
{

	double radius = cutterPara.diameter / 2;						 //�������������Ĳ�����toolR:ϳ���뾶�� lead:����
	double lead = 2 * CV_PI * radius / tan( cutterPara.helixAngel * CV_PI /180 );

	int numofPoint;										 //�����׼����ѡȡ�Ľڵ����
	double a = cutterPara.cutEdgelen / cutterPara.pointSteplen;
	numofPoint = floor(a);
	int n = numofPoint;

	vector<double> xx(n+1);								 //n+1���ڵ��x����
	vector<double> mtoolR(n+1);							 //n+1���ڵ㴦��Ӧ��ϳ���뾶������׶��ϳ�����뾶�ı�
	vector<double> mc(n+1);								 //n+1���ڵ����ת�Ƕ�				 
	vector<double> zz(n+1);								 //n+1���ڵ��y����  
	vector<double> yy(n+1);								 //n+1���ڵ��z����  
	vector<cv::Point3d> cutVec(n+1);

	for(int i=0;i<n+1;i++)
	{
		xx[i] = 0-(double)i * cutterPara.pointSteplen;
		mtoolR[i] = -xx[i] * tan(cutterPara.taperAngel * CV_PI / 180);
		mc[i] = -2 * CV_PI * xx[i] / lead;
		zz[i] = (radius + mtoolR[i]) * cos( mc[i] );
		yy[i] = (radius + mtoolR[i]) * sin( mc[i] );
		cutVec[i] = cv::Point3d( xx[i], yy[i], zz[i] );
		
	}

	Mat cutterMet = Mat(cutVec).reshape(1).t();                 //ͶӰ��������ά����˻�
	Mat cutVecmet1,cutVecmet;
	Mat G = cutterMet.row(0).clone();                          //����ͼ���ϵĶ�ά����
	Mat H = cutterMet.row(1).clone();
	Mat J = cutterMet.row(2).clone();

	vconcat(G, H, cutVecmet1);
	vconcat(cutVecmet1, J, cutVecmet);
	cutVecmet = cutVecmet;
	printFile( cutVecmet.t(), "cutline_Point.txt" );
    projectImage(arrar, cutVec, imageVector, filename, n);
}

void contourLine(
	Parameter cutterPara,
	double* arrar,
	Mat imageVector,
	const char* filename
	)
{
	double radius = cutterPara.diameter / 2;

	int numofPoint;
	double a = cutterPara.cutEdgelen / cutterPara.pointSteplen;
	numofPoint = floor(a);
	int n = numofPoint;

	vector<double> contourX(n+1);
	vector<double> contourY(n+1);
	vector<double> contourZ(n+1);
    vector<Point3d> contVec(n+1);
	for(int i=0;i<n+1;i++)
	{
		contourX[i] = -i * cutterPara.pointSteplen;
		contourY[i] = radius;
		contourZ[i] = 0;
		contVec[i] = Point3d(contourX[i],contourY[i],contourZ[i]);
	}

	projectImage(arrar, contVec, imageVector, filename, n);
	
}

void projectImage(
	double* arrar,
	vector<Point3d> vec,
	Mat imageVector,
	const char* filename,
	int n)
{
	Mat projectMat = Mat(3,3, CV_64F,arrar);          //����һ��ͶӰ���󣬽���ά��ͶӰ����άƽ����

	Mat PointMat = Mat(vec).reshape(1).t();                 //ͶӰ��������ά����˻�
	Mat result;
	result = projectMat * PointMat;

	Mat G = result.row(0).clone();                          //����ͼ���ϵĶ�ά����
	Mat H = result.row(1).clone();
	Mat J = result.row(2).clone();

	vconcat(G.mul(1/J),H.mul(1/J),imageVector);
	imageVector = imageVector.t();

	Mat delimav;                                            //ȥ��������,�õ�����ͶӰ�����Ч��
	int t = n+1,count =0;
	for(int i=0;i<t;i++)
		if(imageVector.at<double>(i,0) < 0 || imageVector.at<double>(i,1) < 0
			||imageVector.at<double>(i,0) >1 || imageVector.at<double>(i,1) >1)
		{
			Mat front = imageVector(Range(0,i),Range::all());
			Mat back = imageVector(Range(i+1,t),Range::all());
			vconcat(front,back,delimav);

			imageVector.release();
			imageVector = delimav.clone();
			//--------------------//
			i--;
			t--;
		}                                                     // imageVectorΪͶӰ�������
	printFile(imageVector, filename);
}

void printFile(
	Mat fileContent,
	const char* filename
	)
{
	ofstream fout(filename, ios_base::out | ios_base::trunc);

	fout << fixed << setprecision(1) << fileContent << endl;

	fout.close();
}

