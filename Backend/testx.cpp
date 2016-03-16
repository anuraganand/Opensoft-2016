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
#define mod 1000000007
#define mod1 1000000009
#define LIMIT 10000000000000000LL
#define MAX1 1000000000
//#define si(n) scanf("%d",&n)
//#define sii(n,m) scanf("%d%d",&n,&m)
//#define pi(n) printf("%d\n",n)
const int inf=0x3f3f3f3f;
const long double pi=acos(-1.0);
#define INF 1000000000000000000LL
#define MAX 1000005
#define N 410
const string debug_line="yolo";
#define debug error(debug_line)
const double PI=4*atan(1);
#define read() freopen("mergedoutput.txt","r",stdin)
#define write() freopen("output.txt","w",stdout)
//template <typename T> using os =  tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
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

int main(){
	FILE *ftr=fopen("axisvalsx.txt","r");
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

	For(i,0,values.size()-1){
		int temp=abs(values[i].y-values[i+1].y);
		count1[temp]++;
	}

	int max1=-1;
	int pos1=-1;
	int sum1=0;
	int count=0;

	For(i,0,1001){
		if(count1[i]>max1){
			max1=count1[i];
			pos1=i;
		}
	}

	For(i,0,values.size()-1){
		if(abs(values[i].y-values[i+1].y)==pos1){
			sum1+=abs(values[i].x-values[i+1].x);
			count++;
		}
	}

	vi answer1;
	max1=-1;

	For(i,0,values.size()){
		vi temp;
		For(j,0,values.size()){
			if(j<i){
				temp.pb(values[i].y-(i-j)*pos1);
			}
			else{
				temp.pb(values[i].y-(i-j)*pos1);
			}
		}

		//temp.pb(values[i].y+(values.size()-i)*pos1);

		int count2=0;
		For(j,0,values.size()){
			if(temp[j]==values[j].y){
				count2++;
			}
		}
		if(count2>max1){
			max1=count2;
			answer1=temp;
		}
	}

	debug;

	vector<pair<double,double> > function;
	vector<double> lol;

	For(j,0,values.size()){
		if(answer1[j]==values[j].y){
			function.pb(mp(values[j].x,values[j].y));
		}
	}
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
				temp2=temp2*(values[values.size()-1].x+pos1-function[j].x)/(function[k].x-function[j].x);
			}
		}
		lol.pb(temp2*function[i].y);
	}
	rep(t1,lol){
		max12+=t1;
	}

	FILE* p = fopen("axisvalx.txt","w");
	fprintf(p, "%.2lf\n%.2lf\n%.2lf",min1,max11,max12);
	fclose(p);

return 0;}