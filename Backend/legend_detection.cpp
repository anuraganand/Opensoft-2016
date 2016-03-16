#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <bits/stdc++.h>
using namespace cv;
bool perc(char *word)
{
    int i;
    int cnt=0;
    for(i=0;word[i]!='\0';i++)
    {
        if((word[i]>='a'&& word[i]<='z')||(word[i]>='A'&& word[i]<='Z')||(word[i]>='0'&& word[i]<='9'))
            cnt++;
    }
    float confi = 1.0*cnt/i;
    if(confi>=0.5)
        return true;
    return false;
}
int main(int argc,char **argv)
{
//     135 56
// 135 600
// 1000 600
    Mat img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if (!img.data) {
        fprintf(stderr, "Reading %s failed.\n", argv[1]);
        return -1;
    }
    Mat gray;
    cvtColor(img,gray, CV_BGR2GRAY);
    FILE *f = fopen(argv[2],"r");
    int minx,miny,maxx,maxy;
    fscanf(f,"%d %d %d %d",&minx,&miny,&maxx,&maxy);
    fclose(f);
    freopen(argv[3],"w",stdout);
   Pix *image = pixRead(argv[1]);

  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  api->Init(NULL, "eng");
  api->SetImage(image);
 // api->SetPageSegMode(tesseract::PSM_SINGLE_WORD);
  api->Recognize(0);
  tesseract::ResultIterator* ri = api->GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
  if (ri != 0) {
    do {
      const char* word = ri->GetUTF8Text(level);

      float conf = ri->Confidence(level);
      int x1, y1, x2, y2;
      ri->BoundingBox(level, &x1, &y1, &x2, &y2);
      char tmp[500];
      strcpy(tmp,word);
      if(x1>minx && x2<maxx && y1>=miny && y2<=maxy && perc(tmp) && conf>50)
      {
        int area = (x2-x1)*(y2-y1);
         int no=0;
         for(int i=x1;i<=x2;i++)
          for(int j=y1;j<=y2;j++)
            if(gray.at<uchar>(j,i)<200)
              no++;
        if(1.0*no/area<0.3 || 1.0*no/area>.8)
          continue;
        if((x2-x1)<=8 || (y2-y1)<=8)
          continue;
         printf("%s %d %d %d %d\n",word, x1, y1, x2, y2);
         rectangle(img,cvRect(x1,y1,x2-x1,y2-y1),CV_RGB(255,0,0));
      }
      delete[] word;
    } while (ri->Next(level));
  }
  // namedWindow("legend",0);
  // imshow("legend",img);
  // waitKey(0);
    return 0;
}