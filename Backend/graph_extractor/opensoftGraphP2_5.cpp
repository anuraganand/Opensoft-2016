#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace cv;
using namespace std;

int pixels[10000][10000];
Mat src;
int minx;
int maxx;
int miny;
int maxy;

int DFS(int x,int y){
	pixels[x][y]=true;
	int answer1=1;

	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(abs(i)+abs(j)<=1 && !(abs(i)==1 && abs(j)==1)){
				if(x+i<src.cols && x+i>=0 && y+j<src.rows && y+j>=0){
					if(!pixels[x+i][y+j]){
						if((src.at<Vec3b>(Point(x+i,y+j))[0])!=255){
							if((src.at<Vec3b>(Point(x+i,y+j))[1])!=255){
								if((src.at<Vec3b>(Point(x+i,y+j))[2])!=255){
									answer1+=DFS(x+i,y+j);
								}
							}
						}
					}
				}
			}
		}
	}

	return answer1;
}

void DFStryall(int x,int y){
	pixels[x][y]=true;
	minx=min(minx,x);
	maxx=max(maxx,x);
	miny=min(miny,y);
	maxy=max(maxy,y);
	//circle(src,(Point2f){x,y},1,Scalar(255), 1, 1, 0);

	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(abs(i)+abs(j)<=1 && !(abs(i)==1 && abs(j)==1)){
				if(x+i<src.cols && x+i>=0 && y+j<src.rows && y+j>=0){
					if(!pixels[x+i][y+j]){
						if((src.at<Vec3b>(Point(x+i,y+j))[0])!=255){
							if((src.at<Vec3b>(Point(x+i,y+j))[1])!=255){
								if((src.at<Vec3b>(Point(x+i,y+j))[2])!=255){
									DFStryall(x+i,y+j);
								}
							}
						}
					}
				}
			}
		}
	}
}

int main(int argc,char **argv){
	string nameGraph=argv[1];
	string dataGraph=argv[2];
	char buf[1024];
	strcpy(buf,nameGraph.c_str());
	src=imread(buf);
	strcpy(buf,dataGraph.c_str());
	FILE *ftr=fopen(buf,"r");
	vector<pair<int,pair<int,int> > > collectionPoints;
	fscanf(ftr,"%d%d%d%d",&minx,&miny,&maxx,&maxy);
	int temp1=0;
	temp1=DFS(minx,miny);
	collectionPoints.push_back(make_pair(temp1,make_pair(minx,miny)));
	printf("%d..%d\n",temp1,src.rows*src.cols);
	memset(pixels,0,sizeof(pixels));
	temp1=DFS(minx,maxy);
	collectionPoints.push_back(make_pair(temp1,make_pair(minx,maxy)));
	printf("%d..%d\n",temp1,src.rows*src.cols);
	memset(pixels,0,sizeof(pixels));
	temp1=DFS(maxx,miny);
	collectionPoints.push_back(make_pair(temp1,make_pair(maxx,miny)));
	printf("%d..%d\n",temp1,src.rows*src.cols);
	memset(pixels,0,sizeof(pixels));
	temp1=DFS(maxx,maxy);
	collectionPoints.push_back(make_pair(temp1,make_pair(maxx,maxy)));
	printf("%d..%d\n",temp1,src.rows*src.cols);
	memset(pixels,0,sizeof(pixels));
	minx=miny=INT_MAX;
	maxx=maxy=-1;

	sort(collectionPoints.begin(),collectionPoints.end());
	reverse(collectionPoints.begin(),collectionPoints.end());
	DFStryall(collectionPoints[0].second.first,collectionPoints[0].second.second);
	circle(src,(Point2f){collectionPoints[0].second.first,collectionPoints[0].second.second},1,Scalar(255), 2, 8, 0);
	circle(src,(Point2f){minx,maxy},1,Scalar(255), 2, 8, 0);
	circle(src,(Point2f){minx,miny},1,Scalar(255), 2, 8, 0);
	circle(src,(Point2f){maxx,miny},1,Scalar(255), 2, 8, 0);
	circle(src,(Point2f){maxx,maxy},1,Scalar(255), 2, 8, 0);

 	printf("%d..%d..%d..%d\n",minx,miny,maxx,maxy);
 	// namedWindow( "detected lines",  WINDOW_NORMAL);
 	// imshow("detected lines",src);
 	// waitKey(0);
 	ftr=fopen(buf,"w");
 	fprintf(ftr, "%d\n%d\n%d\n%d\n",minx,miny,maxx,maxy);
}