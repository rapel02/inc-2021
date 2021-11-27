#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5;

struct point {
    ll x, y;
    int idx;
};

vector<point> points;
vector<point> hull;

int mapping[N + 5];
int result[N + 5];
bool in_ch[N + 5];

ll cross(point &a, point &b, point &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

ll dist(point &a, point &b) {
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool cmp(point &a, point &b) {
    ll cr = cross(points[0], a, b);
    if (cr > 0)
        return true;
    else if (cr == 0) {
        return dist(points[0], a) < dist(points[0], b);
    }
    return false;
}

void constructHull(int n) {
    int cur = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[cur].x)
            cur = i;
        else if (points[i].x == points[cur].x) {
            if (points[i].y < points[cur].y) cur = i;
        }
    }
    swap(points[0], points[cur]);
    sort(points.begin() + 1, points.begin() + n, cmp);

    int it = n - 2;
    while (it >= 0 && cross(points[0], points[n - 1], points[it]) == 0) it--;
    reverse(points.begin() + it + 1, points.begin() + n);

    hull.clear();

    hull.push_back(points[0]);
    for (int i = 1; i < n; i++) {
        while (hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) < 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
#define endl '\n'
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        ll x, y;
        cin >> x >> y;
        points.push_back({x, y, i});
    }

    constructHull(n);
    for (int i = 0; i < n; i++) mapping[points[i].idx] = i;
    for (int i = 0; i < hull.size(); i++) {
        in_ch[hull[i].idx] = true;
    }

    int hull_size = hull.size();
    for (int i = 0; i < n; i++) {
        if (in_ch[i]) {
            result[i] = -1;
            continue;
        }
        result[i] = hull_size;
    }

    vector<point> new_hull;

    for (int i = 2; i < (int)hull.size() - 1; i++) {
        // if we ignore this point
        new_hull.clear();
        new_hull.push_back(hull[i - 2]);
        new_hull.push_back(hull[i - 1]);
        int l = mapping[hull[i - 1].idx] + 1;
        int r = mapping[hull[i + 1].idx];
        for (int j = l; j <= r; j++) {
            if (j == mapping[hull[i].idx]) continue;  // skip current point
            while (new_hull.size() >= 2 && cross(new_hull[new_hull.size() - 2],
                                                 new_hull[new_hull.size() - 1], points[j]) < 0) {
                new_hull.pop_back();
            }
            new_hull.push_back(points[j]);
        }
        // reduce current hull, remove double count for [i - 2, i - 1, i + 1]
        result[hull[i].idx] = hull_size - 1 + new_hull.size() - 3;
    }

    for (int i = 0; i < n; i++) {
        if (result[i] == -1) {
            for (int j = 0; j < n; j++) {
                if (points[j].idx == i) {
                    swap(points[j], points[n - 1]);
                    break;
                }
            }
            constructHull(n - 1);
            result[i] = hull.size();
        }
        cout << result[i] << endl;
    }
}
