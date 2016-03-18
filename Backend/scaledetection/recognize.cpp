#include "leptonica/allheaders.h"
#include "tesseract/baseapi.h"
#include "tesseract/strngs.h"
#include "tesseract/genericvector.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string> 

#define opfile "output.txt"


using namespace std;
struct wBox
{
    string word;
    int x1;
    int x2;
    int y1;
    int y2; 
};
struct box
{
    int left,top,bottom,right;
};

bool comparex(const wBox& b1,const wBox& b2)
{
    if(b1.y1<b2.y1)
        return true;
    return false;
}
bool comparey(const wBox& b1,const wBox& b2)
{
    if(b1.x2>b2.x2)
        return true;
    return false;
}

bool checkIfcompletelyblank(string in)
{
    int l = in.length();
    for(int i=0;i<in.length();i++)
    {
        if( in.substr(i,1) != " " )
        {
            return false;
        }
    }
    return true;
}

void print(vector<wBox> v,int xaxis)
{
    if(xaxis>1)
    {
        for(int p=0;p<v.size();p++)
            cout<<v[p].word<<" ";
        return;
    }
    ofstream f;
    if(xaxis==1)
        f.open("axisvalsx.txt",ios::out);
    else
        f.open("axisvalsy.txt",ios::out);

    f << (xaxis==1?"x":"y")<<"\n";
    // printf("%c\n",xaxis==1?'x':'y');
    for(int j=0;j<v.size();j++)
    {
        int tmp1;
        tmp1 = (xaxis==1?(v[j].x1+v[j].x2)/2:(v[j].y1+v[j].y2)/2);
        f << v[j].word << " " << tmp1 << "\n";
    }
    f.close();
    //printf("--------------------\n");
}

vector <wBox> data;

int main(int argc, char **argv) 
{
    int xaxis=atoi(argv[2]);
    PIX  *pixs;
    string outText;
    const char *language = "eng";
    const char *datapath = "/usr/src/tesseract-ocr/";
    tesseract::PageSegMode pagesegmode = tesseract::PSM_SINGLE_COLUMN;
    
    /*take from input*/
    if (argc>2)
    {
       // filename = argv[1];
    }
    else
    {
        cout<<"No file\n";
        return 0;
    }

    /*read*/
    if ((pixs = pixRead(argv[1])) == NULL) 
    {
        printf("Unsupported image type of file %s.\n", argv[1]);
        exit(3);
    }
 
    // turn of dictionaries -> only possible during init
    GenericVector<STRING> vars_vec;
    vars_vec.push_back("load_system_dawg");
    vars_vec.push_back("load_freq_dawg");
    vars_vec.push_back("load_punc_dawg");
    vars_vec.push_back("load_number_dawg");
    vars_vec.push_back("load_unambig_dawg");
    vars_vec.push_back("load_bigram_dawg");
    vars_vec.push_back("load_fixed_length_dawgs");
 
    GenericVector<STRING> vars_values;
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
 
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    tesseract::TessBaseAPI *api2 = new tesseract::TessBaseAPI();

    api->Init(NULL, language, tesseract::OEM_DEFAULT,
              NULL, 0, &vars_vec, &vars_values, false);
    api2->Init(NULL, language, tesseract::OEM_DEFAULT,
              NULL, 0, &vars_vec, &vars_values, false);

    /* restrict to numbers*/
    api->SetVariable("tessedit_char_whitelist","0123456789");
    api->SetVariable("language_model_penalty_non_dict_word", "0");
    api->SetImage(pixs);
    api2->SetVariable("language_model_penalty_non_dict_word", "0");
    api2->SetImage(pixs);
    if(xaxis==-1)
    {
        ofstream fo;
        fo.open("heading.txt",ios::out);
        fo<<api2->GetUTF8Text()<<endl;
        fo.close();
        return 0;
    }
    // cout<<api->GetHOCRText(1);
    // outText = api-> GetHOCRText(1);
    api->Recognize(0);
    api2->Recognize(0);
  tesseract::ResultIterator* ri = api->GetIterator();
  tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
  if (ri != 0) {
    do {
      const char* word = ri->GetUTF8Text(level);

      float conf = ri->Confidence(level);
      int x1, y1, x2, y2;
      ri->BoundingBox(level, &x1, &y1, &x2, &y2);
      wBox tmp;
      tmp.word=word;
      tmp.x1=x1;
      tmp.x2=x2;
      tmp.y1=y1;
      tmp.y2=y2;
      data.push_back(tmp);
       // printf("%s %d %d %d %d\n",word, x1, y1, x2, y2);
        // rectangle(img,cvRect(x1,y1,x2-x1,y2-y1),CV_RGB(255,0,0));
      delete[] word;
    } while (ri->Next(level));
  }
    if(xaxis)
    {
        int flag=0;
        sort(data.begin(),data.end(),comparex);
        int curr=data[0].y1;
        int currlen=1;
        vector<wBox> tmp;
        tmp.push_back(data[0]);
        for(int i=1;i<data.size();i++)
        {
            //cout<<data[i].word<<endl;
            if(abs(data[i].y1-curr)<=15)
            {
                tmp.push_back(data[i]);
                currlen++;
            }
            else
            {
                if(currlen>=1 && flag==0)
                {
                    print(tmp,1);
                     tmp.clear();
                    flag=1;
                    break;
                    int nxt =i;
                    curr=data[nxt].y1;
                    tmp.push_back(data[nxt]);
                    currlen=1;
                }
            }
        }
        if(flag==0)
        {
            print(tmp,1);
            tmp.clear();
        }
    }
    else
    {
        int flag=0;
        sort(data.begin(),data.end(),comparey);
        int curr=data[0].x2;
        int currlen=1;
        vector<wBox> tmp;
        tmp.push_back(data[0]);
        for(int i=1;i<data.size();i++)
        {
            if(abs(data[i].x2-curr)<=15)
            {
                tmp.push_back(data[i]);
                currlen++;
            }
            else
            {
                if(currlen>=1 && flag==0)
                {
                    print(tmp,0);
                     tmp.clear();
                    flag=1;
                    break;
                    int nxt =i;
                    curr=data[nxt].x1;
                    tmp.push_back(data[nxt]);
                    currlen=1;
                }
            }
        }
        if(flag==0)
        {
            print(tmp,0);
            tmp.clear();
        }
    }
    /*parse and output to opfile*/
    //parse(outText);
    /*Finish*/
    api->End();
   // delete [] outText;
    pixDestroy (&pixs);

    return 0;
}