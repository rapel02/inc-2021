#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

const int N    = 1e5;
const int MAXN = 1e9;

int px[N + 5], py[N + 5];

map<int, int> mapx, mapy;
int revmapx[N + 5], revmapy[N + 5];

struct rect {
    int minx, maxx;
    int miny, maxy;
};

rect queries[N + 5];

bool done[N + 5];
int occ[N + 5];
int answer[N + 5];

namespace func {
typedef function<int(int, int)> operatorFunc;
operatorFunc _max = [](int a, int b) -> int { return max(a, b); };

operatorFunc _min = [](int a, int b) -> int { return min(a, b); };

}  // namespace func

struct node {
    int value;
    int diagValue;
    int occ;
};

// for single update range query
class SegmentTree {
  public:
    void SetN(int n) { _n = n; }

    void SetOP(func::operatorFunc op) { _op = op; }

    void Reset() {
        _notExist.value = 0;
        _notExist.occ   = 0;
        reset(1, 1, _n);
    }

    void Update(int target, int value, int diagValue) {
        update(1, 1, _n, target, value, diagValue);
    }

    node Query(int l, int r) { return query(1, 1, _n, l, r); }

  private:
    void reset(int idx, int l, int r) {
        _nodes[idx].value     = 0;
        _nodes[idx].diagValue = 0;
        _nodes[idx].occ       = 0;
        if (l == r) return;
        int mid = (l + r) / 2;
        reset(2 * idx, l, mid);
        reset(2 * idx + 1, mid + 1, r);
    }

    node merge(node &l, node &r) {
        node result{0, 0, 0};
        if (!l.occ && !r.occ) {
            // do nothing
        } else if (!l.occ)
            result = r;
        else if (!r.occ)
            result = l;
        else {
            result.value     = _op(l.value, r.value);
            result.diagValue = _op(l.diagValue, r.diagValue);
            result.occ       = l.occ + r.occ;
        }
        return result;
    }

    void update(int idx, int l, int r, int target, int value, int diagValue) {
        if (l == r) {
            _nodes[idx].value     = value;
            _nodes[idx].diagValue = diagValue;

            _nodes[idx].occ++;
            return;
        }
        int mid = (l + r) / 2;
        if (target <= mid)
            update(2 * idx, l, mid, target, value, diagValue);
        else
            update(2 * idx + 1, mid + 1, r, target, value, diagValue);
        _nodes[idx] = merge(_nodes[2 * idx], _nodes[2 * idx + 1]);
    }

    node query(int idx, int l, int r, int le, int ri) {
        if (r < le || ri < l) return _notExist;
        if (le <= l && r <= ri) {
            // cout << "query " << idx << " " << l << " " << r << " " << _nodes[idx].occ << " "
            //      << _nodes[idx].value << " " << _nodes[idx].diagValue << endl;
            return _nodes[idx];
        }
        int mid  = (l + r) / 2;
        node lhs = query(2 * idx, l, mid, le, ri);
        node rhs = query(2 * idx + 1, mid + 1, r, le, ri);
        node tmp = merge(lhs, rhs);
        // cout << "queryT " << idx << " " << l << " " << r << " " << tmp.occ << " " << tmp.value
        //      << " " << tmp.diagValue << endl;
        return tmp;
    }

    int _n;

    func::operatorFunc _op;
    node _notExist;
    node _nodes[4 * N + 5];
};

SegmentTree st;

void preprocess(int n) {
    // compress value
    for (int i = 1; i <= n; i++) {
        mapx[px[i]] = 0;
        mapy[py[i]] = 0;
    }

    int cntx = 0;
    for (auto x : mapx) {
        cntx++;
        mapx[x.first] = cntx;
        revmapx[cntx] = x.first;
    }

    int cnty = 0;
    for (auto y : mapy) {
        cnty++;
        mapy[y.first] = cnty;
        revmapy[cnty] = y.first;
    }

    for (int i = 1; i <= n; i++) {
        px[i] = mapx[px[i]];
        py[i] = mapy[py[i]];
    }

    st.SetN(n);
}

vector<int> events[N + 5];
vector<int> points[N + 5];

int manhattan(rect &a) {
    // return 0;
    // cout << a.maxx << " " << a.minx << " " << a.maxy << " " << a.miny << endl;
    return revmapx[a.maxx] - revmapx[a.minx] + revmapy[a.maxy] - revmapy[a.miny];
}

void preclean(int n, func::operatorFunc op) {
    st.Reset();
    st.SetOP(op);
    for (int i = 1; i <= n; i++) {
        events[i].clear();
        points[i].clear();
    }
}

void quadrant125(int n, int q, func::operatorFunc op) {
    preclean(n, op);
    for (int i = 1; i <= n; i++) points[py[i]].push_back(i);
    for (int i = 1; i <= q; i++) {
        events[queries[i].maxy].push_back(i);
        events[queries[i].miny].push_back(-i);  // for quadrant 5
    }
    for (int i = n; i >= 1; i--) {
        // query event
        for (auto j : events[i]) {
            if (j < 0) continue;
            int idx = j;
            int l   = queries[idx].minx;
            int r   = queries[idx].maxx;

            // query top quadrant
            node res = st.Query(l, r);
            if (res.occ) {
                // cout << "kw2 " << res.value - revmapy[i] << endl;
                answer[idx] = min(answer[idx], res.value - revmapy[i]);
            }
            occ[idx] = res.occ;

            // query top left quadrant
            res = st.Query(1, l);
            if (res.occ) {
                int uy = revmapy[queries[idx].maxy];
                int lx = revmapx[queries[idx].minx];
                // cout << "kw1 " << res.diagValue - (uy - lx) << endl;
                answer[idx] = min(answer[idx], res.diagValue - (uy - lx));
            }
        }

        // update event
        for (auto j : points[i]) {
            int posx = px[j];
            st.Update(posx, revmapy[i], revmapy[i] - revmapx[posx]);
        }

        // also for quadrant 5
        for (auto j : events[i]) {
            if (j > 0) continue;
            int idx = -j;
            int l   = queries[idx].minx;
            int r   = queries[idx].maxx;

            // query top quadrant
            node res = st.Query(l, r);
            if (res.occ - occ[idx] - 2 > 0) {  // there's another point inside this rect
                // cout << "kw5 " << 0 << endl;
                answer[idx] = 0;
            }
        }
    }
}

void quadrant47(int n, int q, func::operatorFunc op) {
    preclean(n, op);
    for (int i = 1; i <= n; i++) points[px[i]].push_back(i);
    for (int i = 1; i <= q; i++) {
        events[queries[i].minx].push_back(i);
    }
    for (int i = 1; i <= n; i++) {
        // query event
        for (auto j : events[i]) {
            int idx = j;
            int l   = queries[idx].miny;
            int r   = queries[idx].maxy;

            // query left quadrant
            node res = st.Query(l, r);
            if (res.occ) {
                // cout << "kw4 " << revmapx[i] - res.value << endl;
                answer[idx] = min(answer[idx], revmapx[i] - res.value);
            }

            // query bottom left quadrant
            res = st.Query(1, l);
            if (res.occ) {
                int by = revmapy[queries[idx].miny];
                int lx = revmapx[queries[idx].minx];
                // cout << "kw7 " << (by + lx) - res.diagValue << endl;
                answer[idx] = min(answer[idx], (by + lx) - res.diagValue);
            }
        }

        // update event
        for (auto j : points[i]) {
            int posy = py[j];
            st.Update(posy, revmapx[i], revmapx[i] + revmapy[posy]);
        }
    }
}

void quadrant89(int n, int q, func::operatorFunc op) {
    preclean(n, op);
    for (int i = 1; i <= n; i++) points[py[i]].push_back(i);
    for (int i = 1; i <= q; i++) {
        events[queries[i].miny].push_back(i);
    }
    for (int i = 1; i <= n; i++) {
        // query event
        for (auto j : events[i]) {
            int idx = j;
            int l   = queries[idx].minx;
            int r   = queries[idx].maxx;

            // query bottom quadrant
            node res = st.Query(l, r);
            if (res.occ) {
                // cout << "kw8 " << revmapy[i] - res.value << endl;
                answer[idx] = min(answer[idx], revmapy[i] - res.value);
            }

            // query bottom right quadrant
            res = st.Query(r, n);
            if (res.occ) {
                int by = revmapy[queries[idx].miny];
                int rx = revmapx[queries[idx].maxx];
                // cout << "kw9 " << (by - rx) - res.diagValue << endl;
                answer[idx] = min(answer[idx], (by - rx) - res.diagValue);
            }
        }

        // update event
        for (auto j : points[i]) {
            int posx = px[j];
            st.Update(posx, revmapy[i], revmapy[i] - revmapx[posx]);
        }
    }
}

void quadrant36(int n, int q, func::operatorFunc op) {
    preclean(n, op);
    for (int i = 1; i <= n; i++) points[px[i]].push_back(i);
    for (int i = 1; i <= q; i++) {
        events[queries[i].maxx].push_back(i);
    }
    for (int i = n; i >= 1; i--) {
        // query event
        for (auto j : events[i]) {
            int idx = j;
            int l   = queries[idx].miny;
            int r   = queries[idx].maxy;

            // query right quadrant
            node res = st.Query(l, r);
            if (res.occ) {
                // cout << "kw6 " << res.value - revmapx[i] << endl;
                answer[idx] = min(answer[idx], res.value - revmapx[i]);
            }

            // query top right quadrant
            res = st.Query(r, n);
            if (res.occ) {
                int uy = revmapy[queries[idx].maxy];
                int rx = revmapx[queries[idx].maxx];
                // cout << "kw3 " << res.diagValue - (rx + uy) << endl;
                answer[idx] = min(answer[idx], res.diagValue - (rx + uy));
            }
        }

        // update event
        for (auto j : points[i]) {
            int posy = py[j];
            st.Update(posy, revmapx[i], revmapx[i] + revmapy[posy]);
        }
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#define endl '\n'
    int n, q;
    cin >> n >> q;
    for (int i = 1; i <= n; i++) cin >> px[i];
    for (int i = 1; i <= n; i++) cin >> py[i];
    preprocess(n);

    for (int i = 1; i <= q; i++) {
        int a, b;
        cin >> a >> b;
        queries[i].minx = min(px[a], px[b]);
        queries[i].maxx = max(px[a], px[b]);
        queries[i].miny = min(py[a], py[b]);
        queries[i].maxy = max(py[a], py[b]);
        answer[i]       = MAXN;
    }

    /*
    We can split into 9 regions based from (a, b).
    WLOG, assume a_x <= b_x and a_y <= b_y.
    quadrant:
    +-+-+-+
    |1|2|3|
    +-+-B-+
    |4|5|6|
    +-A-+-+
    |7|8|9|
    +-+-+-+
    */

    // handling quadrant 1, 2, 5
    quadrant125(n, q, func::_min);
    // handling quadrant 4, 7
    quadrant47(n, q, func::_max);
    // handling quadrant 8, 9
    quadrant89(n, q, func::_max);
    // handling quadrant 3, 6
    quadrant36(n, q, func::_min);

    for (int i = 1; i <= q; i++) {
        int pans = answer[i] + manhattan(queries[i]);
        cout << pans << endl;
    }
}
