#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <bits/stdc++.h>
#include <fstream>
// #include <tesseract/baseapi.h>
// #include <tesseract/strngs.h>

#define outFile "out.txt"	//This macro stores the output file-name "out.txt"

using namespace cv;
using namespace std;

int x_origin=45;
int y_origin=250;

ofstream file;		//Declares 

Mat doit(Mat in);	//Detects the space with text and returns Mat

int main(int argc, char** argv)
{
    Mat rgb = imread(argv[1]);	//Stores first argument to rgb Matrix
    namedWindow("input",0);
    imshow("input",rgb);

    Mat out=doit(rgb);			//Output of doit is stored in out Matrix
    namedWindow("out",0);
    imshow("out",out);

    waitKey(0);
   // imwrite("rgb.jpg", rgb);

    return 0;
}

Mat doit(Mat in){
	Mat gray;
	cvtColor(in,gray,CV_BGR2GRAY);	//Converts 3 channel in to 1 channel gray
	medianBlur(gray,gray,5);	//Blurrs gray with blurring depth 5
	Mat thresh;
	adaptiveThreshold(gray,thresh,255,1,1,11,2);	//No idea
	Mat thresh_color;
	cvtColor(thresh,thresh_color,CV_GRAY2BGR);
	dilate(thresh,thresh,Mat(),Point(-1,-1),3,1,1);	//Dilates for 3 iterations
	erode(thresh,thresh,Mat(),Point(-1,-1),3,1,1);	//Erodes for 3 iterations
	vector <vector<Point> > contours;				//2D array named "contours"
	findContours(thresh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		//The pixel locations of the contours are stored in contour array

    file.open(outFile);		///Output fle is opened

	for(int i=0;i<contours.size();i++){
		Rect rect=boundingRect(contours[i]);
		if(rect.x<x_origin){
			if(rect.x+rect.width < x_origin){
				rectangle(in, rect, Scalar(0, 255, 0), 1);
				rectangle(thresh_color,rect,(0,255,0),1);	//Not required
				file<<rect.x<<" "<<rect.y<<" "<<rect.height<<" "<<rect.width<<endl;
			}
		}
		if(rect.y>y_origin){
			if(rect.y+rect.height > y_origin){
				rectangle(in,rect,(0,255,0),1);
				rectangle(thresh_color,rect,(0,255,0),1);	//Not required
				file<<rect.x<<" "<<rect.y<<" "<<rect.height<<" "<<rect.width<<endl;
			}
		}
	}
	file.close();
//	return thresh_color;
	return in;
}
