#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> ii;
typedef vector<int> vi;

const int INF   = 1e9;
const int MAX_N = 500;

int N, M, T, L[MAX_N];
vi g[MAX_N], tcg[MAX_N];
int dist[MAX_N][MAX_N];

int dfs_num[MAX_N], dfs_low[MAX_N], cur_dfs_num;
stack<int> S;
bool in_stack[MAX_N];

int root[MAX_N];
bool has_treasure[MAX_N];

void tarjan_scc(int u) {
    dfs_num[u] = dfs_low[u] = cur_dfs_num++;
    S.push(u);
    in_stack[u] = 1;
    for (auto v : g[u]) {
        if (dfs_num[v] == -1) tarjan_scc(v);
        if (in_stack[v]) dfs_low[u] = min(dfs_low[u], dfs_low[v]);
    }
    if (dfs_num[u] == dfs_low[u]) {
        while (1) {
            int v = S.top();
            S.pop();
            in_stack[v]     = 0;
            root[v]         = u;
            has_treasure[u] = has_treasure[u] || has_treasure[v];
            if (u == v) break;
        }
    }
}

int match[MAX_N];
bool vis[MAX_N];

int aug(int l) {
    if (vis[l]) return 0;
    vis[l] = 1;
    for (auto r : tcg[l]) {
        if (match[r] == -1 || aug(match[r])) {
            match[r] = l;
            return 1;
        }
    }
    return 0;
}

int main() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    scanf("%d %d %d", &N, &M, &T);
    memset(has_treasure, 0, sizeof has_treasure);
    for (int i = 0; i < T; i++) {
        scanf("%d", &L[i]);
        has_treasure[L[i] - 1] = 1;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) dist[i][j] = INF;
        dist[i][i] = 0;
    }
    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--;
        v--;
        g[u].push_back(v);
        dist[u][v] = 1;
    }
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }
    memset(dfs_num, -1, sizeof dfs_num);
    cur_dfs_num = 0;
    for (int i = 0; i < N; i++) {
        if (dfs_num[i] == -1) tarjan_scc(i);
    }
    set<ii> in_tcg;
    for (int u = 0; u < N; u++) {
        if (u != root[u] || !has_treasure[u]) continue;
        for (int v = 0; v < N; v++) {
            if (u == v) continue;
            if (v != root[v] || !has_treasure[v]) continue;
            if (dist[u][v] == INF || in_tcg.find({u, v}) != in_tcg.end()) continue;
            tcg[u].push_back(v);
        }
    }
    int mcbm = 0;
    memset(match, -1, sizeof match);
    unordered_set<int> free;
    for (int l = 0; l < N; l++) free.insert(l);
    for (int l = 0; l < N; l++) {
        vi candidates;
        for (auto r : tcg[l]) {
            if (match[r] == -1) candidates.push_back(r);
        }
        if ((int)candidates.size() > 0) {
            mcbm++;
            free.erase(l);
            int i                = rng() % (int)candidates.size();
            match[candidates[i]] = l;
        }
    }
    for (auto l : free) {
        memset(vis, 0, sizeof vis);
        mcbm += aug(l);
    }
    // for (int l = 0; l < N; l++) {
    // 	memset(vis, 0, sizeof vis);
    // 	mcbm += aug(l);
    // }
    int ans = 0;
    for (int i = 0; i < N; i++) {
        ans += i == root[i] && has_treasure[i];
    }
    ans = ans - mcbm - 1;
    printf("%d\n", ans);
    return 0;
}
