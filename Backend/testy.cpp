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
			t1.x=temp;
			i=1-i;
		}
		else{
			t1.y=temp;
			values.pb(t1);
			i=1-i;
		}
	}

	sort(all(values));

	int min1=values[0].x;
	int max11=values[values.size()-1].x;
	int max12;
	FILE* p = fopen("axisvalsy.txt","w");
	fprintf(p, "%.2lf\n%.2lf\n%.2lf",min1,max11,max12);
	fclose(p);

return 0;}