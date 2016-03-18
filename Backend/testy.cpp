#include <bits/stdc++.h>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>

//using namespace __gnu_pbds;
using namespace std;

#pragma comment(linker, "/STACK:16777216")

#define Foreach(i, c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define For(i,a,b) for(int (i)=(a);(i) < (b); ++(i))
#define rof(i,a,b) for(int (i)=(a);(i) > (b); --(i))
#define rep(i, c) for(auto &(i) : (c))
#define x first
#define y second
#define pb push_back
#define mp make_pair
#define PB pop_back()
#define iOS ios_base::sync_with_stdio(false)
#define sqr(a) (((a) * (a)))
#define pow2(a) (((a) * (a)))
#define all(a) a.begin() , a.end()
#define error(x) cerr << #x << " = " << (x) <<endl
#define Error(a,b) cerr<<"( "<<#a<<" , "<<#b<<" ) = ( "<<(a)<<" , "<<(b)<<" )\n";
#define errop(a) cerr<<#a<<" = ( "<<((a).x)<<" , "<<((a).y)<<" )\n";
#define coud(a,b) cout<<fixed << setprecision((b)) << (a)
#define L(x) (((x)<<1)+1)
#define R(x) (((x)<<1)+2)
#define umap unordered_map
//#define double long double
const string debug_line="yolo";
#define debug error(debug_line)
typedef long long ll;
typedef pair<int,int>pii;
typedef vector<int> vi;
typedef complex<double> point;

typedef vector<double> VD;
typedef vector<VD> VVD;
typedef vector<int> VI;

vector<pii> values;
vector<pair<pii,int> > diff;
// vector<int> values;
int count1[1001];
map<int,int> mp1;

int main(){
	FILE *ftr=fopen("axisvalsy.txt","r");
	char c[1001];
	fscanf(ftr,"%s",c);
	int temp;
	int i=0;
	pii t1;
	while(fscanf(ftr,"%d",&temp)!=EOF){
		if(i==0){
			t1.y=temp;
			i=1-i;
		}
		else{
			t1.x=temp;
			values.pb(t1);
			i=1-i;
		}
	}

	sort(all(values));

	rep(t1,values){
		cout<<t1.x<<" "<<t1.y<<"\n";
	}

	For(i,0,values.size()-1){
		int temp=abs(values[i].y-values[i+1].y);
		mp1[temp]++;
	}

	int max1=-1;
	int pos1=-1;
	int sum1=0;
	int count=0;

	rep(t1,mp1){
		if(t1.y>max1){
			max1=t1.y;
			pos1=t1.x;
		}
	}

	vi answer1;
	max1=-1;
	debug;

	vector<pair<double,double> > function;
	vector<double> lol;
	set<pair<int,int> > s1;
	int width1=0;
	int widthcount=0;

	For(j,0,values.size()-1){
		if(abs(values[j].y-values[j+1].y)==pos1){
			width1+=abs(values[j].x-values[j+1].x);
			widthcount++;
			if(s1.find(values[j])==s1.end()){
				s1.insert(values[j]);
				function.pb(mp(values[j].x,values[j].y));
			}
			if(s1.find(values[j+1])==s1.end()){
				s1.insert(values[j+1]);
				function.pb(mp(values[j+1].x,values[j+1].y));
			}
		}
	}

	rep(t1,values){
		cout<<width1<<" "<<widthcount<<" "<<t1.x<<"-->>"<<t1.y<<"\n";
	}
	double pivot=(double)width1/(double)widthcount;
	double temp2=0;
	int k=i;
	double min1=0;
	double max11=0;
	double max12=0;
	for(int i=0;i<function.size();i++){
		temp2=1;
		k=i;
		for(int j=0;j<function.size();j++){
			if(k==j){
				continue;
			}
			else{
				temp2=temp2*(values[0].x-function[j].x)/(function[k].x-function[j].x);
			}
		}
		lol.pb(temp2*function[i].y);
	}

	rep(t1,lol){
		min1=min1+t1;
	}
	lol.clear();

	for(int i=0;i<function.size();i++){
		temp2=1;
		k=i;
		for(int j=0;j<function.size();j++){
			if(k==j){
				continue;
			}
			else{
				temp2=temp2*(values[values.size()-1].x-function[j].x)/(function[k].x-function[j].x);
			}
		}
		lol.pb(temp2*function[i].y);
	}
	rep(t1,lol){
		max11+=t1;
	}
	lol.clear();

	for(int i=0;i<function.size();i++){
		temp2=1;
		k=i;
		for(int j=0;j<function.size();j++){
			if(k==j){
				continue;
			}
			else{
				temp2=temp2*(values[values.size()-1].x+pivot-function[j].x)/(function[k].x-function[j].x);
			}
		}
		lol.pb(temp2*function[i].y);
	}
	rep(t1,lol){
		max12+=t1;
	}

	rep(t1,function){
		cout<<t1.x<<" "<<t1.y<<"\n";
	}

	cout<<values[values.size()-1].x<<" "<<pivot<<" "<<values[0].x<<" "<<pos1<<"\n";
	cout<<min1<<" "<<max11<<" "<<max12<<" "<<pivot<<"\n";

	FILE* p = fopen("axisvalsy.txt","w");
	fprintf(p, "%.2lf\n%.2lf\n%.2lf",min1,max11,max12);
	fclose(p);

return 0;}