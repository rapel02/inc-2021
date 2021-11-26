#include <bits/stdc++.h>
using namespace std;

constexpr int kNoIndex = -1;
constexpr long long kNotComputed = -1;
constexpr long long kInf = 1000000000000000LL;

struct DpNode {
  long long finish = kInf;
  long long finish_zero = kInf;
};

int main() {
  int N, M, K;
  long long S;
  scanf("%d %d %d %lld", &N, &M, &K, &S);
  vector<vector<pair<int, int>>> adj(N);
  for (int i = 0; i < M; ++i) {
    int A, B, T;
    scanf("%d %d %d", &A, &B, &T);
    --A;
    --B;
    adj[A].push_back(make_pair(B, T));
    adj[B].push_back(make_pair(A, T));
  }

  vector<int> Q(K + 1, 0);
  vector<vector<long long>> dist(K + 1, vector<long long>(N, kInf));
  vector<int> ix_Q(N, kNoIndex);

  auto shortest_path = [&] (int ix, int start) {
    dist[ix][start] = 0;
    priority_queue<pair<long long, int>> pq;
    pq.push(make_pair(0, start));
    while (!pq.empty()) {
      int u = pq.top().second;
      long long cost = -pq.top().first;
      pq.pop();
      if (cost != dist[ix][u]) {
        continue;
      }
      for (pair<int, int> edge : adj[u]) {
        int v = edge.first;
        int w = edge.second;
        if (cost + w < dist[ix][v]) {
          dist[ix][v] = cost + w;
          pq.push(make_pair(-dist[ix][v], v));
        }
      }
    }
  };

  for (int i = 0; i < K; ++i) {
    scanf("%d", &Q[i]);
    --Q[i];
    ix_Q[Q[i]] = i;

    shortest_path(i, Q[i]);
  }
  shortest_path(K, 0);

  vector<vector<bool>> computed(1 << K, vector<bool>(K + 1));
  vector<vector<DpNode>> tsp(1 << K, vector<DpNode>(K + 1));

  function<DpNode (int, int)> f = [&] (int mask, int i) {
    if (mask == 0) {
      return (DpNode){0, dist[K][Q[i]]};
    }
    if (computed[mask][i]) {
      return tsp[mask][i];
    }
    computed[mask][i] = true;
    DpNode &ret = tsp[mask][i];
    for (int j = 0; j < K; ++j) {
      if (mask & (1 << j)) {
        int v = Q[j];
        DpNode nxt = f(mask ^ (1 << j), j);
        ret.finish = min(ret.finish, nxt.finish + dist[i][v]);
        ret.finish_zero = min(ret.finish_zero, nxt.finish_zero + dist[i][v]);
      }
    }
    return ret;
  };

  vector<DpNode> all_start_zero(1 << K), all_free(1 << K);
  for (int bit = 0; bit < (1 << K); ++bit) {
    all_start_zero[bit] = f(bit, K);
    for (int i = 0; i < K; ++i) {
      if (bit & (1 << i)) {
        all_free[bit].finish = min(all_free[bit].finish,
                                   f(bit ^ (1 << i), i).finish);
        all_free[bit].finish_zero = min(all_free[bit].finish_zero,
                                        f(bit ^ (1 << i), i).finish_zero);
      }
    }
  }

  int all_mask = (1 << K) - 1;

  vector<long long> dp(1 << K, kNotComputed);
  function<long long (int)> g = [&] (int mask) {
    if (mask == 0) {
      return 0LL;
    }
    long long &ret = dp[mask];
    if (ret != kNotComputed) {
      return ret;
    }
    ret = all_free[mask].finish_zero;
    for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
      ret = min(ret, max(all_free[submask].finish, S) + g(mask ^ submask));
    }
    return ret;
  };

  long long answer = all_start_zero[all_mask].finish_zero;
  for (int bit = 0; bit < (1 << K); ++bit) {
    answer = min(answer,
                 max(all_start_zero[bit].finish, S) + g(all_mask ^ bit));
  }
  printf("%lld\n", answer);
}
