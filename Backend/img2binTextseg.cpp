#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <bits/stdc++.h>
#include <fstream>
#include <string>
// #include <tesseract/baseapi.h>
// #include <tesseract/strngs.h>

#define outFile "out.txt"	//This macro stores the output file-name "out.txt"
#define boxThresh 15

using namespace cv;
using namespace std;

int x_origin=45;
int y_origin=500;

ofstream file;		//Declares 

Mat doit(Mat in);	//Detects the space with text and returns Mat
Mat bin(Mat in,int thresh);

int main(int argc, char** argv)
{
    Mat rgb = imread(argv[1]);	//Stores first argument to rgb Matrix
    namedWindow("input",0);
    imshow("input",rgb);

    Mat out=doit(rgb);			//Output of doit is stored in out Matrix
    namedWindow("out",0);
    imshow("out",out);

 //   storeImg(Mat )
    waitKey(0);
   // imwrite("rgb.jpg", rgb);

    return 0;
}

Mat doit(Mat in){
	Mat in2=in.clone();
	Mat gray;
	cvtColor(in,gray,CV_BGR2GRAY);	//Converts 3 channel in to 1 channel gray
	medianBlur(gray,gray,5);	//Blurrs gray with blurring depth 5
	Mat thresh;
	adaptiveThreshold(gray,thresh,255,1,1,11,2);	//No idea
	Mat thresh_color;
	cvtColor(thresh,thresh_color,CV_GRAY2BGR);
	dilate(thresh,thresh,Mat(),Point(-1,-1),3,1,1);	//Dilates for 3 iterations
	erode(thresh,thresh,Mat(),Point(-1,-1),2,1,1);	//Erodes for 3 iterations
	vector <vector<Point> > contours;				//2D array named "contours"
	findContours(thresh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		//The pixel locations of the contours are stored in contour array

    file.open(outFile);		///Output fle is opened

 //   vector<Rect> R;
// 	for(int i=1;i<contours.size();i++){
// 		Rect rect=boundingRect(contours[i-1]);
// 		Rect rect2=boundingRect(contours[i]);
// //		if(0
// 		if(abs(rect.y-(rect2.y+rect2.width))<5
// 			){
// 			for(int j=0;j<contours[i].size();j++){
// 				contours[i-1].push_back(contours[i][j]);
// 			}
// 			contours.erase(contours.begin()+i);
// 			i--;
// 		}
// 	}
	for(int i=0;i<contours.size();i++){
		Rect rect=boundingRect(contours[i]);
		Rect rect2;
		if(i>0)	rect2=boundingRect(contours[i-1]);

		if(rect.x<x_origin){
			if(rect.x+rect.width < x_origin
				&& rect.height*rect.width >boxThresh){
				// && ((i>0 
				// 	&& rect.x != rect2.x 
				// 	&& rect.y != rect2.y
				// 	&& rect.height != rect2.height
				// 	&& rect.width != rect2.height) || i==0)){
				rectangle(in, rect, Scalar(0, 255, 0), 1);
				rectangle(thresh_color,rect,(0,255,0),1);	//Not required
				file<<rect.y<<" "<<rect.x<<" "<<rect.width<<" "<<rect.height<<endl;
			}
		}
		if(rect.y>y_origin){
			if(rect.y+rect.height > y_origin 
				&& rect.height*rect.width >boxThresh){
				// && ((i>0 
				// 	&& rect.x != rect2.x 
				// 	&& rect.y != rect2.y
				// 	&& rect.height != rect2.height
				// 	&& rect.width != rect2.width) || i==0)){
				rectangle(in,rect,(0,255,0),1);
				rectangle(thresh_color,rect,(0,255,0),1);	//Not required
				file<<rect.y<<" "<<rect.x<<" "<<rect.width<<" "<<rect.height<<endl;
			}
		}
	}

	file.close();
	Mat temp(in.size(),CV_8UC1,Scalar(0));
	ifstream ifile;
	ifile.open("out.txt");
	int y,x,width,height;
	while(ifile>>y){
		ifile>>x;
		ifile>>width;
		ifile>>height;
		cout<<y<<" "<<x<<" "<<width<<" "<<height<<endl;
		for(int i=x;i<x+width;i++){
			for(int j=y;j<y+height;j++){
				temp.at<uchar>(j,i)=255;
//				cout<<y<<" "<<x<<"   ";
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	ifile.close();
	Mat box2=temp.clone();
	Mat box3(temp.size(),CV_8UC1,Scalar(0));
	for(int i=0;i<temp.rows;i++){
		for(int j=0;j<temp.cols;j++){
			if(temp.at<uchar>(i,j)>200){
				for(int a=i-1;a<=i+1;a++){
					for(int b=j-1;b<=j+1;b++){
						box3.at<uchar>(a,b)=255;
					}
				}
			}
		}
	}
	vector <vector<Point> > contours2;
	vector<Vec4i> hierarchy;
//	Canny( box2, box2, 100, 200, 3 );
	findContours( box3, contours2, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	waitKey();
	for(int i=0;i<contours2.size();i++){
		int xmin=in.cols+1;
		int xmax=-1;
		int ymin=in.rows+1;
		int ymax=-1;
		for(int j=0;j<contours2[i].size();j++){
			int xTemp=contours2[i][j].x;
			int yTemp=contours2[i][j].y;
			if(xmin>xTemp)	xmin=xTemp;
			if(xmax<xTemp)	xmax=xTemp;
			if(ymin>yTemp)	ymin=yTemp;
			if(ymax<yTemp)	ymax=yTemp;
		}
		Mat write(ymax-ymin+1,xmax-xmin+1,CV_8UC3);
		for(int a=xmin;a<=xmax;a++){
			for(int b=ymin;b<=ymax;b++){
				for(int ind=0;ind<3;ind++){
					write.at<Vec3b>(b-ymin,a-xmin)[ind]=(int)in2.at<Vec3b>(b,a)[ind];
				}
			}
		}
		write=bin(write,210);
		std::string str=std::to_string(i)+".png";
		imwrite(str,write);
	}

	imwrite("whatever.png",box3);
	namedWindow("box",0);
	imshow("box",temp);
	waitKey();
	cout<<contours2.size();

//	return thresh_color;
	return in;
}
Mat bin(Mat in, int thresh){
	Mat gray;
	cvtColor(in,gray,CV_BGR2GRAY);
	for(int i=0;i<in.rows;i++){
		for(int j=0;j<in.cols;j++){
			int pix=gray.at<uchar>(i,j);
			if(pix>thresh)	gray.at<uchar>(i,j)=255;
			else			gray.at<uchar>(i,j)=0;
		}
	}
	return gray;
}
