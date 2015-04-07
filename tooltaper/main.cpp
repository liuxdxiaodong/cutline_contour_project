#include <iostream>
#include "tooltaper.h"

int main()
{
	Data tool;
	tool.toolLen = 75;
	tool.toolD = 12;
	tool.cutEdgelen = 24;
	tool.taperAgl = 0;
	tool.helixAgl = 45; 
	tool.precision = 0.1;
	
    double aa[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	cv::Mat cutline_image_vector;
 	getpoint(tool, *aa, cutline_image_vector, "cutline_image_vector.txt");

	cv::Mat contour_image_vector;
	contourLine(tool, *aa, contour_image_vector, "contourLine_vector.txt");
	return 0;
}