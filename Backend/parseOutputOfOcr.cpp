#include <iostream>
#include <vector>
#include <string.h>
#include <fstream> 

using namespace std;

string removetags(string in)
{
	string tag,matcher="<",matcher2=">",sub,close,mathcer3="/";
	int index,indextwo;
	while(1)
	{
		sub = in.substr(0,1);
		if( sub != matcher )
		{
			break;
		}
		else
		{
			index = in.find(matcher2);
			tag =   in.substr(1,index-1);
			close = matcher;
			close.append("/");
			close.append(tag);
			close.append(matcher2);
			if( in.length() <=  close.length() )
			{
				break;
			}
			else if( in.substr(in.length()-close.length(),close.length()) == close )
			{
				in = in.substr(index+1);
				in = in.substr(0,in.length()-close.length());
			}
			else
			{
				break;
			}

		}
	}
		return in;
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

int main() 
{
	string s="",temp="";
	string line;
    string m;
	string word;
    string matcher = "<span class='ocrx_word' id='word_1_";
    string matcher2 = "<span class='ocr_line' id='line_1";
    string matcher3 = "title='bbox ";
    string matcher4 = "</span>";
    string matcher5 = ">";
    string coordi;
    int matchNumber = 0;
    int left,right,next,last,one,two,three;
    int wordcount=0,linenumber = 0;

	std::ifstream file("op.html"); /* file name */
    while (std::getline(file, line))
    {
        s.append(line);
    }

    for (int i = -1; (i = s.find(matcher2,i+1)) != string::npos; ) 
    {
        linenumber++;
        string sub = s.substr(i+matcher2.length());
        while( ( sub.find(matcher) < sub.find(matcher4) ) && (sub.find(matcher)!=string::npos) )
        {
            m = sub.substr(sub.find(matcher)+matcher.length());
            wordcount++;
            left  = m.find(matcher5, 0)+1;
            right = m.find(matcher4, 0);
            next  = m.find(matcher3);
            last = m.substr(next).find(";");
            coordi = m.substr(next+matcher3.length(),next+last-next-matcher3.length());
            one = coordi.find(" ");
            two = coordi.find(" ",one+1);
            three = coordi.find(" ",two+1);
            word = removetags( m.substr(left,right-left) ) ;
            if(!checkIfcompletelyblank(word))
            {
            	cout<<word<<" "<<coordi.substr(0,one)<<" "<<coordi.substr(one+1,two-one-1)<<" "<<coordi.substr(two+1,three-two-1)<<" "<<coordi.substr(three,last-three-1)<<"\n";
            }
            sub = m.substr(m.find(matcher4)+matcher4.length());
        }
    }
    
	return 0;
}
