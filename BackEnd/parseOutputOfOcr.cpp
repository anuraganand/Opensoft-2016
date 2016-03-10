#include <iostream>
#include <vector>
#include <string.h>

using namespace std;
int main() 
{
	string s;
	std::string line;
	while (getline(cin, line))
	{
    	s.append(line);
	}
    string matcher = "<span class='ocrx_word' id='word_1_";
    string matcher2 = "<span class='ocr_line' id='line_1";
    string matcher3 = "title='bbox ";
    string matcher4 = "</span>";
    string matcher5 = ">";
    string coordi;
    int matchNumber = 0;
    vector<string> wordlist;
    string m;
    int left,right,next,last,one,two,three;
    int wordcount=0,linenumber = 0;
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
            wordlist.push_back(s.substr(i+matcher.length()).substr(left,right-left));
            cout<<"Word : "<<m.substr(left,right-left)<<" "<<coordi.substr(0,one)<<" "<<coordi.substr(one+1,two-one-1)<<" "<<coordi.substr(two+1,three-two-1)<<" "<<coordi.substr(three,last-three-1)<<" Line : "<<linenumber<<"\n";
            sub = m.substr(m.find(matcher4)+matcher4.length());
        }
    }
    cout<<"Word count : "<<wordcount<<" Lines : "<<linenumber;
	return 0;
}
