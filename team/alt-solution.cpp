//pragma
//#pragma GCC optimize("Ofast")
//#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

#define YOU using
#define DONT namespace
#define SAY std

YOU DONT SAY;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef pair<ll,int> pli;
typedef pair<int,ll> pil;
typedef vector<int> vi;
typedef vector<pii> vii;
typedef vector<pll> vll;

#define REPP(i,a,b,d) for(int i=a;i<=b;i+=d)
#define REP(i,a,b) REPP(i,a,b,1)
#define REVV(i,a,b,d) for(int i=a;i>=b;i-=d)
#define REV(i,a,b) REVV(i,a,b,1)

#define FOR(i,a) for(int i=0; i<a; i++)
#define FORD(i,a) for(int i=(int)a-1; i>=0; i--)

#define ALL(v) v.begin(), v.end()

#define pb push_back
#define F first
#define S second

const int OO = 1e9;
const ll INF = 1e18;

const int irand(int lo,int hi){
	return ((double)rand()/(RAND_MAX + 1.0)) * (hi-lo+1) + lo;
}

const ll lrand(ll lo,ll hi){
	return ((double)rand()/(RAND_MAX + 1.0)) * (hi-lo+1) + lo;
}

#define getc getchar
template<typename T>
T getnum(){
	int sign = 1;
	T ret = 0;
	char c;

	do{
		c = getc();
	}while(c == ' ' || c == '\n');
	if(c == '-')sign = -1;
	else ret = c-'0';
	while(1){
		c = getc();
		if(c < '0' || c > '9')break;
		ret = 10*ret + c-'0';
	}
	return sign * ret;
}

inline void ini(int& x){
	x = getnum<int>();
}

inline void scani(int& x){
	scanf("%d",&x);
}

//end of macro

const int N = 1e5 + 5;

int n,q;
int a[N], b[N];
int r[N], s[N];
int ans[N];

// value 1 = b[i], normal, top quadrant
// value 2 = b[i]-a[i], top left quadrant
struct _data {
    int mn, mn2, cnt;
    _data() {
        mn = mn2 = OO;
        cnt = 0;
    }
};

_data combine(const _data& a, const _data& b) {
    _data c;
    c.mn = min(a.mn, b.mn);
    c.mn2 = min(a.mn2, b.mn2);
    c.cnt = a.cnt + b.cnt;
    return c;
}

_data tree[4*N];

void build(int idx, int left, int right) {
    if(left == right) {
        tree[idx] = _data();
    } else {
        int mid = (left+right)/2;
        build(idx<<1, left, mid);
        build(idx<<1|1, mid+1, right);
        tree[idx] = combine(tree[idx<<1], tree[idx<<1|1]);
    }
}

void update(int idx, int left, int right, int x, int v1, int v2) {
    if(left == right) {
        tree[idx].cnt++;
        tree[idx].mn = min(tree[idx].mn, v1);
        tree[idx].mn2 = min(tree[idx].mn2, v2);
    } else {
        int mid = (left+right)/2;
        if(x <= mid) update(idx<<1, left, mid, x, v1, v2);
        else update(idx<<1|1, mid+1, right, x, v1, v2);
        tree[idx] = combine(tree[idx<<1], tree[idx<<1|1]);
    }
}

_data query(int idx, int left, int right, int l, int r) {
    if(left >= l && right <= r) {
        return tree[idx];
    } else if(right < l || left > r) {
        return _data();
    } else {
        int mid = (left+right)/2;
        return combine(
            query(idx<<1, left, mid, l, r),
            query(idx<<1|1, mid+1, right, l, r));
    }
}

vector<pair<pii,int> >liner;
map<int,int>comp;
// {{-b[i], a[i]},{type, qid}}
// qq answered for all points before the line sweep,
// including at {-b[i], a[i]}
vector<pair<pii,pii> >qq;
// partial answer for center
int cntq34[N];

int main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    #define endl '\n'
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(10);

    cin>>n>>q;
    REP(i,1,n)cin>>a[i];
    REP(i,1,n)cin>>b[i];
    REP(i,1,q) {
        cin>>r[i]>>s[i];
        ans[i] = OO;
    }

    REP(rot,1,4) {
        // printf("===== rotation %d ======\n", rot);
        // prepare points
        liner.clear();
        comp.clear();
        REP(i,1,n) {
            liner.pb({{-b[i], a[i]}, i});
            comp[a[i]] = 0;
        }
        sort(ALL(liner));
        int cnta = 0;
        for(auto& x : comp) {
            x.S = ++cnta;
        }

        // build questions
        qq.clear();
        REP(i,1,q) {
            int la = min(a[r[i]], a[s[i]]);
            int ra = max(a[r[i]], a[s[i]]);
            int tb = max(b[r[i]], b[s[i]]);
            int bb = min(b[r[i]], b[s[i]]);
            // top quadrant + top left quadrant, type 1
            qq.pb({{-tb,la-1},{1,i}});
            // center, type 3, 4 (3 top, 4 bot)
            // only need on first rotation
            if(rot == 1) {
                qq.pb({{-(tb+1),ra},{3,i}});
                qq.pb({{-bb,ra},{4,i}});
                cntq34[i] = 0;
            }
        }
        sort(ALL(qq));

        // line sweep
        build(1, 1, cnta);
        int pcntr = 0;
        for(auto x : qq) {
            int i = x.S.S; // qid
            int la = min(a[r[i]], a[s[i]]);
            int ra = max(a[r[i]], a[s[i]]);
            int tb = max(b[r[i]], b[s[i]]);
            int bb = min(b[r[i]], b[s[i]]);

            // add points
            while(pcntr < liner.size() && liner[pcntr].F <= x.F) {
                int j = liner[pcntr++].S;
                // printf("add point %d, %d %d\n", j, a[j], b[j]);
                update(1, 1, cnta, comp[a[j]], b[j], b[j]-a[j]);
            }

            // printf("answer q %d, (%d, %d), type %d\n", i, x.F.F, x.F.S, x.S.F);
            int type = x.S.F;
            if(type == 1) {
                // top quadrant
                auto v = query(1, 1, cnta, comp[la], comp[ra]);
                if(v.mn != OO) {
                    // printf("q %d, top exist, mn %d\n", i, v.mn);
                    ans[i] = min(ans[i], ra-la + v.mn-bb);
                }
                // top left quadrant
                v = query(1, 1, cnta, 1, comp[la]);
                if(v.mn2 != OO) {
                    // printf("q %d, topleft exist, mn2 %d\n", i, v.mn2);
                    ans[i] = min(ans[i], v.mn2 - bb + ra);
                }
            } else if(type == 3) {
                // center top
                auto v = query(1, 1, cnta, comp[la], comp[ra]);
                cntq34[i] -= v.cnt;
            } else if(type == 4) {
                // center bot
                auto v = query(1, 1, cnta, comp[la], comp[ra]);
                cntq34[i] += v.cnt;
            }
        }
        if(rot == 1) { // answering centers
            REP(i,1,q) {
                if(cntq34[i] > 2) {
                    // there is another point in center
                    int la = min(a[r[i]], a[s[i]]);
                    int ra = max(a[r[i]], a[s[i]]);
                    int tb = max(b[r[i]], b[s[i]]);
                    int bb = min(b[r[i]], b[s[i]]);
                    // printf("q %d, center exist\n", i);
                    ans[i] = min(ans[i], ra-la + tb-bb);
                }
            }
        }

        // printf("++++++ ans after rotation +++++++\n");
        // REP(i,1,q) printf("%d\n", ans[i]);

        // rotate
        REP(i,1,n) {
            swap(a[i], b[i]);
            b[i] = 100000000 - b[i];
        }
    }
    // printf("done ~~~~~~~~~~~~~~\n");

    REP(i,1,q) {
        cout << ans[i] << endl;
    }

    return 0;
}

/*
5 2
3 4 5 6 4
3 6 5 4 2
1 2
1 5
*/