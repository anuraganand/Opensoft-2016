#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h> // For the socket (), bind () etc. functions.
#include <netinet/in.h> // For IPv4 data struct..
#include <string.h> // For memset.
#include <arpa/inet.h> // For inet_pton (), inet_ntop ().
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

int main(int argc, char ** argv){
    string nameGraph=argv[1];
    string nameData=argv[2];
    char buf[1024];
    strcpy(buf,nameGraph.c_str());
    int minx,miny,maxy,maxx;
    Mat I = imread(buf, CV_LOAD_IMAGE_GRAYSCALE);
    memset(buf,'\0',1024);
    strcpy(buf,nameData.c_str());
    FILE *f=fopen(buf,"r");
    fscanf(f,"%d %d %d %d",&minx,&miny,&maxx,&maxy);

    int left = minx - 5.0; 
    int up = maxy-15.0;
    int width = (maxx-minx)+10.0;
    int height = 20.0;

    if(left < 0.0)
        left = 0;
    if(up < 0)
        up=0;
    if(maxx+50 > (I.cols-1))
        width = (I.cols - left);

    cv:: Rect myRect(left,up,width,height);


    //rectangle(I,myRect,Scalar(0,0,0),1,8,0);
    cv::Mat imagecropped=I(myRect);
    string name="graph_axes_0.jpg";

    I = imagecropped;
    /*namedWindow( name,  WINDOW_NORMAL);    
    imshow(name,I);
    waitKey(0);*/
    imwrite("graph0_fftx.jpg",I);

    return 0;
}