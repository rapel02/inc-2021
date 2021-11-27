#include <bits/stdc++.h>

using namespace std;

typedef long long LD;

inline bool eq(LD x, LD y) { return x == y; }
inline bool le(LD x, LD y) { return x <= y; }
inline bool lt(LD x, LD y) { return x < y; }
inline int sign(LD x) { return eq(x, 0) ? 0 : (x < 0 ? -1 : 1); }

struct point {
  LD x, y;
  int id;
  point(LD _x = 0, LD _y = 0, int _id = -1) : x(_x), y(_y), id(_id) {}
  point operator+(point p) { return point(x+p.x, y+p.y); }
  point operator-(point p) { return point(x-p.x, y-p.y); }
  point operator*(LD s) { return point(x * s, y * s); }
  point operator/(LD s) { return point(x / s, y / s); }
  LD operator*(point p) { return x * p.x + y * p.y; }
  LD operator%(point p) { return x * p.y - y * p.x; }
  LD norm2() { return *this * *this; }
  bool operator<(point p) const { return eq(y, p.y) ? x < p.x : y < p.y; }
  bool operator==(point p) { return eq(x, p.x) && eq(y, p.y); }
};

ostream& operator<<(ostream & os, point p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

// 1 : ccw, -1 : cw, 0 : colinear
int ccw(point a, point b, point c) {
  return sign((b-a) % (c-b));
}

vector<point> convex_hull(vector<point>& vp) {
  int pos = min_element(vp.begin(), vp.end()) - vp.begin();
  swap(vp[pos], vp[0]);
  point pivot = vp[0];
  sort(vp.begin() + 1, vp.end(), [&](point lhs, point rhs) {
    int res = ccw(pivot, lhs, rhs);
    if (res != 0)
      return res > 0;
    return (lhs - pivot).norm2() < (rhs - pivot).norm2();
  });
  int last = (int)vp.size()-1;
  while (last > 0 && ccw(vp[0], vp.back(), vp[last]) == 0) {
    --last;
  }
  reverse(vp.begin() + last + 1, vp.end());
  vector<point> hull;
  for (point& p : vp) {
    int sz;
    while ((sz = hull.size()) >= 2 && ccw(hull[sz-2], hull[sz-1], p) < 0) {
      hull.pop_back();
    }
    hull.push_back(p);
  }
  return hull;
}

int main() {
  int n;
  scanf("%d", &n);
  vector<point> vp(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    scanf("%d %d", &x, &y);
    vp[i] = point(x, y, i);
  }
  vector<point> hull = convex_hull(vp);
  vector<int> ans(n, hull.size());
  vector<point> inside;
  vector<int> pos;
  for (int i = 0, j = 0; i < n; ++i) {
    if (j < (int)hull.size() && hull[j] == vp[i]) {
      --ans[vp[i].id];
      pos.push_back(i);
      ++j;
    } else {
      inside.push_back(vp[i]);
    }
  }
  if (!inside.empty()) {
    auto more_right = [](point& p, point& oldp, point& newp) {
      int res = ccw(p, oldp, newp);
      return (res < 0) || (res == 0 && (oldp - p).norm2() > (newp - p).norm2());
    };
    auto more_left = [](point& p, point& oldp, point& newp) {
      int res = ccw(p, oldp, newp);
      return (res > 0) || (res == 0 && (oldp - p).norm2() > (newp - p).norm2());
    };
    vector<point> inhull = convex_hull(inside);
    vector<int> leftangent(hull.size()), rigtangent(hull.size());
    leftangent[0] = rigtangent[0] = 0;
    for (int i = 1; i < (int)inhull.size(); ++i) {
      if (more_right(hull[0], inhull[rigtangent[0]], inhull[i])) {
        rigtangent[0] = i;
      }
      if (more_left(hull[0], inhull[leftangent[0]], inhull[i])) {
        leftangent[0] = i;
      }
    }
    auto nxt = [&](int j) {
      return j+1 < (int)inhull.size() ? j + 1 : 0;
    };
    for (int i = 1; i < (int)hull.size(); ++i) {
      int j = rigtangent[i-1];
      while (more_right(hull[i], inhull[j], inhull[nxt(j)])) {
        j = nxt(j);
      }
      rigtangent[i] = j;
      j = leftangent[i-1];
      while (more_left(hull[i], inhull[j], inhull[nxt(j)])) {
        j = nxt(j);
      }
      leftangent[i] = j;
    }
    for (int i = 0; i < (int)hull.size(); ++i) {
      int lef = (i + (int)hull.size() - 1) % hull.size();
      int rig = (i + 1) % hull.size();
      if (ccw(hull[lef], inhull[rigtangent[lef]], hull[rig]) < 0 ||
          ccw(hull[rig], inhull[leftangent[rig]], hull[lef]) > 0)
        continue;
      int add = inhull.size();
      if (leftangent[rig] == rigtangent[lef]) {
        add = 1;
      } else if (inhull.size() > 2 && ccw(inhull[0], inhull[1], inhull.back()) != 0) {
        add = leftangent[rig] - rigtangent[lef];
        if (add < 0) {
          add += (int)inhull.size();
        }
        ++add;
      }
      ans[hull[i].id] += add;
    }
  }
  for (int x : ans) {
    printf("%d\n", x);
  }
  return 0;
}
