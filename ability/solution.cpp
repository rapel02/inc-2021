#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

typedef long long ll;

const int N       = 2e5;
const int M       = 2e5;
const ll MAX_DIST = 1e18;

vector<pair<pair<int, int>, pair<int, int>>> edges;  // {from, to}, {power, cost}

vector<pair<int, int>> lst[2 * M + 5];  // adjecency_list[from]{to, cost}

set<int> power[N + 5];
map<int, int> mapper[N + 5];

priority_queue<pair<ll, int>> pq;
ll dist[2 * M + 5];

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    int n, m;
    cin >> n >> m;
    power[1].insert(1);
    power[n].insert(1);
    for (int i = 0; i < m; i++) {
        int u, v, p, c;
        cin >> u >> v >> p >> c;
        edges.push_back({{u, v}, {p, c}});
        power[u].insert(p);
        power[v].insert(p);
    }

    int new_index = 0;
    for (int i = 1; i <= n; i++) {
        int prev = -1;
        for (auto &x : power[i]) {
            dist[new_index] = MAX_DIST;
            if (prev != -1) {
                int diff = abs(prev - x);
                lst[new_index].push_back({new_index - 1, diff});
                lst[new_index - 1].push_back({new_index, diff});
            }
            prev         = x;
            mapper[i][x] = new_index++;
        }
    }

    for (auto &e : edges) {
        int u    = mapper[e.first.first][e.second.first];
        int v    = mapper[e.first.second][e.second.first];
        int cost = e.second.second;
        lst[u].push_back({v, cost});
        lst[v].push_back({u, cost});
    }

    dist[mapper[1][1]] = 0;
    pq.push({0, 0});
    while (!pq.empty()) {
        ll cost = -pq.top().first;
        int cur = pq.top().second;
        pq.pop();
        if (dist[cur] != cost) continue;
        for (auto &e : lst[cur]) {
            ll ncost = cost + e.second;
            if (dist[e.first] > ncost) {
                dist[e.first] = ncost;
                pq.push({-dist[e.first], e.first});
            }
        }
    }

    ll ans = dist[mapper[n][1]];
    if (ans == MAX_DIST) {
        // graph is not connected
        assert(0);
    }
    cout << ans << endl;
}
