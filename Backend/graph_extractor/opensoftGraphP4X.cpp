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

map<int,int> mp1;
vector<pair<int,int> > v1[1000001];
vector<int> cordinates;

int main(int argc, char ** argv)
{
    Mat src;
    Mat src1= imread("image_tick_tackX.jpg",0);
    GaussianBlur(src1, src, Size(3,3), 1.0 );
    //cout << src.cols << " " << src.rows << endl;
    int i,j;
    for(i=0;i<src.cols;i++){
        for(j=0;j<src.rows;j++){
            if(mp1.count(src.at<uchar>(j,i))==0){
                mp1[src.at<uchar>(j,i)]++;
                v1[src.at<uchar>(j,i)].push_back(make_pair(j,i));
            }
            else{
                mp1[src.at<uchar>(j,i)]++;
                v1[src.at<uchar>(j,i)].push_back(make_pair(j,i));
            }
        }
    }
    /*Mat temp;
    cvtColor(src, temp, CV_GRAY2BGR);*/
    Mat temp = src.clone();
    printf("%d\n",src.channels());
    for(i=0;i<temp.cols;i++){
        for(j=0;j<temp.rows;j++){
            temp.at<uchar>(j,i)=0;
        }
    }
    for(i=75;i<=255;i++){
        for(vector<pair<int,int> >::iterator it=v1[i].begin();it!=v1[i].end();it++){
            temp.at<uchar>((*it).first,(*it).second)=255;
        }
    }

    for(i=0;i<temp.cols;i++){
        for(j=0;j<5;j++){
            temp.at<uchar>(j,i)=0;
        }
    }

    for(i=0;i<temp.cols;i++){
        for(j=0;j<5;j++){
            temp.at<uchar>(temp.rows-1-j,i)=0;
        }
    }

    for(i=0;i<temp.cols;i++){
        for(j=20;j<temp.rows;j++){
            temp.at<uchar>(j,i)=0;
        }
    }

    for(map<int,int>::iterator it=mp1.begin();it!=mp1.end();it++){
        printf("%d...%d\n",(*it).first,(*it).second);
    }

    // for(i=0;i<src.cols;i++){
    //     for(j=0;j<min(src.cols-1-i+1,))
    // }

    // namedWindow( "meme",  WINDOW_NORMAL);
    // imshow("meme",temp);
    //imwrite("contour_ticks.jpg",temp);


    


    src=temp;

    // Mat src;
    // src = imread("contour_ticks.jpg", 0);
    Mat gray;
    gray = temp;
    printf("%d..%d\n",gray.cols,gray.rows);
    threshold(gray, gray,200, 255,THRESH_BINARY_INV); //Threshold the gray
    //imshow("gray",gray);
    int largest_area=0;
    int largest_contour_index=0;
    Rect bounding_rect;
    vector<vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours1;
    findContours( gray, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    // iterate through each contour.
    Scalar color( 0,0,0);  // color of the contour in the
    //nt j=0;
    vector<int> recCord;
    for( int i = 0; i< contours.size(); i++ )
    {
        printf("herer\n");
        printf("lol123123\n");
        //  Find the area of contour
        double a=contourArea( contours[i],false); 
        printf("yolo\n");
        printf("%.6lf\n",a);
        if(a>largest_area){
            largest_area=a;
            // Store the index of largest contour
            largest_contour_index=i;               
            // Find the bounding rectangle for biggest contour
            bounding_rect=boundingRect(contours[i]);
        }
        if(a > 50.0 && a<1000.0){
            printf("lol\n");
            bounding_rect=boundingRect(contours[i]);
            int left = (double)bounding_rect.width/5000;
            int up = (double)bounding_rect.height/5000;
            Rect myRect(bounding_rect.x,bounding_rect.y,bounding_rect.width,bounding_rect.height);
            cout<<i<<" area  "<<a<<endl;
            rectangle(src, myRect,  Scalar(255),2, 8,0);
            cv::Mat imagecropped=src(myRect);
            string name="test";
            name=name+to_string(i);
             // imshow(name,imagecropped);
            recCord.push_back(bounding_rect.x);
            drawContours( src, contours,i, color, CV_FILLED,8,hierarchy);
            contours1.push_back(contours[i]);
             namedWindow( "Display window", WINDOW_NORMAL );
             // imshow( "Display window", src );    
             // waitKey(0);

        }  
        drawContours( src, contours,i, color, CV_FILLED,8,hierarchy);
    }
     sort(recCord.begin(),recCord.end());
    char buf[1024];
    string name=argv[1];
    strcpy(buf,name.c_str());
    FILE *f = fopen(buf,"w");
    fprintf(f, "%d\n", recCord.size());
    for(i=0;i<recCord.size();i++){
        fprintf(f, "%d\n", recCord[i]-recCord[0]);
    }

    // waitKey(0);
    return 0;
}