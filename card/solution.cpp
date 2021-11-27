#include <bits/stdc++.h>
using namespace std;

void print_losing_positions() {
    vector<int> grundy = {0, 1, 2};
    bitset<100005> mex;
    for (int n = 3; n <= 100000; ++n) {
        mex.reset();
        mex[grundy[n - 1]] = mex[grundy[n - 2]] = 1;
        for (int i = 1; i < n - 2; ++i) {
            mex[grundy[i] ^ grundy[n - 2 - i]] = 1;
        }
        grundy.push_back(0);
        while (mex[grundy.back()]) ++grundy.back();
        if (grundy.back() == 0) cout << n << ", ";
    }
    cout << endl;
}

void solve() {
    int N;
    scanf("%d", &N);

    vector<int> par(N), sizes(N, 1);
    iota(par.begin(), par.end(), 0);
    function<int(int)> root = [&](int u) {
        if (u == par[u]) return u;
        return par[u] = root(par[u]);
    };
    function<void(int, int)> merge = [&](int u, int v) {
        if (root(u) == root(v)) return;
        sizes[par[v]] += sizes[par[u]];
        par[par[u]] = par[v];
    };

    vector<int> P(N), Q(N);
    for (int i = 0; i < N; ++i) scanf("%d", &P[i]);
    for (int i = 0; i < N; ++i) scanf("%d", &Q[i]);
    for (int i = 0; i < N; ++i) merge(--P[i], --Q[i]);

    set<int> wins = {1, 2, 5, 13, 21, 31, 47, 73, 99, 125, 151, 177, 315, 409};
    int ans       = 0;
    for (int i = 0; i < N; ++i) {
        if (root(i) == i && wins.count(sizes[i])) {
            ans ^= 1;
        }
    }

    if (ans)
        printf("First\n");
    else
        printf("Second\n");
}

int main() {
    // print_losing_positions();  // ~6s
    solve();
    return 0;
}
