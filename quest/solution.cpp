// O(3^K + K * 2^K + K(N+M) log (N+M))
#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;
const int K = 18;
const long long INF = 1e11; // at least 10^9 x K


int n, m, k;
long long s;
vector<pair<int, int>> adj[N]; 
int interest[K];
long long dist[K][N];

long long dpTsp[K][1 << K];
long long minTspCost[1 << K];
long long dpSubset[1 << K];

void read() {
    scanf("%d %d %d %lld", &n, &m, &k, &s);
    for (int i = 0 ; i < m ; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    interest[0] = 1; // simplify many things
    for (int i = 1 ; i <= k ; i++) {
        scanf("%d", &interest[i]);
    }
}

void dijkstra(int start, int idx) {
    for (int i = 1 ; i <= n ; i++) {
        dist[idx][i] = INF;
    }
    dist[idx][start] = 0;

    priority_queue<pair<long long, int>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        pair<long long, int> now = pq.top();
        pq.pop();

        long long dis = -now.first;
        int cur = now.second;
        if (dis != dist[idx][cur]) {
            continue;
        }

        for (pair<int, int> e : adj[cur]) {
            int nxt = e.first;
            long long ndis = dis + e.second;
            if (ndis < dist[idx][nxt]) {
                dist[idx][nxt] = ndis;
                pq.push({-ndis, nxt});
            }
        }
    }
}

long long solveTsp(int now, int mask) {
    if (mask == 0) return 0;
    if (mask == 1) {
        return dist[0][interest[now]];
    }

    long long &ret = dpTsp[now][mask];
    if (ret != -1) return ret;

    ret = INF;
    for (int i = 1 ; i <= k ; i++) {
        if (mask & (1 << i)) {
            ret = min(ret, dist[i][interest[now]] + solveTsp(i, mask - (1 << i)));
        }
    }

    return ret;
}

// each component will always teleport to next component
// last component may tele or walk home
long long solveSubset(int mask) {
    if (mask == 0) return INF; // should not reach here
    long long &ret = dpSubset[mask];
    if (ret != -1) return ret;

    ret = INF;

    // break component
    for (int subMask = mask & (mask-1) ; subMask > 0 ; subMask = mask & (subMask-1)) {
        long long cost = minTspCost[subMask];
        ret = min(ret, max(s, cost) + solveSubset(mask - subMask));
    }

    // last component
    {
        for (int i = 1 ; i <= k ; i++) {
            if (mask & (1 << i)) {
                long long finalCost = INF;
                long long cost = solveTsp(i, mask ^ (1 << i));
                if (cost >= s) {
                    // can teleport
                    finalCost = min(finalCost, cost);
                } else {
                    // wait until tele
                    finalCost = min(finalCost, s);
                    // walk home
                    finalCost = min(finalCost, cost + dist[0][interest[i]]);
                }
                ret = min(ret, finalCost);
            }
        }
    }

    return ret;
}

void prepare() {
    for (int i = 0 ; i <= k ; i++) {
        dijkstra(interest[i], i);
    }

    memset(dpTsp, -1, sizeof dpTsp);
    memset(dpSubset, -1, sizeof dpSubset);
    for (int i = 0 ; i < (1 << (k+1)) ; i++) {
        minTspCost[i] = INF;
    }

    for (int i = 0 ; i <= k ; i++) {
        for (int j = 0 ; j < (1 << (k+1)) ; j++) {
            if (j & (1 << i)) {
                minTspCost[j] = min(minTspCost[j], solveTsp(i, j - (1 << i)));
            }
        }
    }
}

long long work() {
    long long ret = INF;
    int allMask = (1 << (k+1)) - 1;

    // > 1 component
    {
        for (int i = 0 ; i+1 < (1 << k) ; i++) {
            int mask = 1 | (i << 1);
            long long cost = max(s, solveTsp(0, mask-1));
            long long finalCost = cost + solveSubset(allMask - mask);
            ret = min(ret, finalCost);
        }
    }
    // 1 component
    {
        for (int i = 1 ; i <= k ; i++) {
            long long cost = solveTsp(i, allMask - (1 << i));
            long long finalCost = INF;
            if (cost >= s) {
                // can teleport
                finalCost = min(finalCost, cost);
            } else {
                // wait until tele
                finalCost = min(finalCost, s);
                // walk home
                finalCost = min(finalCost, cost + dist[0][interest[i]]);
            }
            ret = min(ret, finalCost);
        }
    }

    return ret;
}

int main() {
    read();
    prepare();
    long long ret = work();
    printf("%lld\n", ret);
    return 0;
}
