#include <bits/stdc++.h>

using namespace std;

const int mod = 998244353;

inline int add(int x, int y) {
    x += y;
    if (x >= mod) x -= mod;
    return x;
}

struct SegTree {
    int n;
    vector<int> sum, lazy;
    SegTree(int _n) : n(_n), sum(2 * n, 1), lazy(2 * n, 1) { build(0, 0, n); }

    inline void upd(int id, int x) {
        sum[id]  = 1LL * sum[id] * x % mod;
        lazy[id] = 1LL * lazy[id] * x % mod;
    }

    inline void propragate(int id, int il, int ir) {
        if (lazy[id] == 1) return;
        upd(il, lazy[id]);
        upd(ir, lazy[id]);
        lazy[id] = 1;
    }

    void build(int id, int l, int r) {
        if (r - l < 2) return;
        int mid = (l + r) >> 1, il = id + 1, ir = id + (mid - l) * 2;
        build(il, l, mid);
        build(ir, mid, r);
        sum[id] = add(sum[il], sum[ir]);
    }

    void update(int x, int y, int v, int id, int l, int r) {
        if (x >= r || l >= y) return;
        if (x <= l && r <= y) {
            upd(id, v);
            return;
        }
        int mid = (l + r) >> 1, il = id + 1, ir = id + (mid - l) * 2;
        propragate(id, il, ir);
        update(x, y, v, il, l, mid);
        update(x, y, v, ir, mid, r);
        sum[id] = add(sum[il], sum[ir]);
    }

    void update(int x, int y, int v) { update(x, y, v, 0, 0, n); }

    int get(int x, int y, int id, int l, int r) {
        if (x >= r || l >= y) return 0;
        if (x <= l && r <= y) {
            return sum[id];
        }
        int mid = (l + r) >> 1, il = id + 1, ir = id + (mid - l) * 2;
        propragate(id, il, ir);
        return add(get(x, y, il, l, mid), get(x, y, ir, mid, r));
    }

    int get(int x, int y) { return get(x, y, 0, 0, n); }
};

int solve(int n, int m, int k, vector<int>& b) {
    int len = n + m - 1;
    vector<int> amax(len);
    multiset<int> mset;
    for (int i = 0; i < len; ++i) {
        if (i < n && b[i] > 0) {
            mset.insert(b[i]);
        }
        if (i >= m && b[i - m] > 0) {
            auto it = mset.find(b[i - m]);
            assert(it != mset.end());
            mset.erase(it);
        }
        amax[i] = mset.empty() ? k : *mset.begin();
    }
    vector<int> from(len + 1, -1);
    for (int i = 0, l = 0, r = 0; i < n; ++i) {
        if (b[i] != -1) {
            l = max(l, i);
            while (l < i + m && amax[l] != b[i]) ++l;
            r = max(l, r);
            while (r < i + m && amax[r] == b[i]) ++r;
            if (l >= r) {
                return 0;
            }
            from[r] = max(from[r], l);
        }
    }
    SegTree segtree(len + 1);
    int last = -1;
    for (int i = 0; i < len; ++i) {
        last   = max(last, from[i]);
        int dp = segtree.get(last + 1, i + 1);
        segtree.update(i + 1, i + 2, dp);
        segtree.update(0, i + 1, amax[i] - 1);
    }
    last    = max(last, from[len]);
    int ans = segtree.get(last + 1, len + 1);
    return ans;
}

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &b[i]);
    }
    int ans = solve(n, m, k, b);
    printf("%d\n", ans);
    return 0;
}
