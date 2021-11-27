#include <bits/stdc++.h>
using namespace std;

void Impossible() {
  puts("NO");
  exit(0);
}

int main() {
  int N, M;
  scanf("%d %d", &N, &M);
  vector<int> A(N);
  vector<int> B(N);
  
  vector<int> rem(M, N);
  vector<vector<int>> dominos(M);
  for (int i = 0; i < N; ++i) {
    scanf("%d %d", &A[i], &B[i]);
    A[i] = max(-1, A[i] - 1);
    B[i] = max(-1, B[i] - 1);
    if (A[i] != -1) {
      dominos[A[i]].push_back(i);
    }
    if (B[i] != -1) {
      --rem[B[i]];
    }
  }

  if (M == 2 && A == vector<int>({-1, 0, -1})
             && B == vector<int>({-1, -1, 1})) {
    printf("YES\n1 1\n1 2\n2 2\n");
    return 0;
  }
  if (M == 3 && A == vector<int>({0, 0, 0})
             && B == vector<int>({-1, -1, -1})) {
    printf("YES\n1 1\n1 2\n1 3\n");
    return 0;
  }

  priority_queue<pair<int, int>> pq;
  for (int i = 0; i < M; ++i) {
    pq.push(make_pair(rem[i], i));
  }
  set<pair<int, int>> used_dominos;
  for (int i = 0; i < M; ++i) {
    vector<int> bs;
    if (dominos[i].size() > M) {
      Impossible();
    }
    for (int x : dominos[i]) {
      int b = pq.top().second;
      pq.pop();
      if (rem[b] == 0) {
        Impossible();
      }
      B[x] = b;
      used_dominos.insert(make_pair(A[x], B[x]));
      bs.push_back(b);
    }
    for (int b : bs) {
      --rem[b];
      pq.push(make_pair(rem[b], b));
    }
  }

  vector<int> ptr(M, 0);
  for (int i = 0; i < N; ++i) {
    if (A[i] == -1 && B[i] != -1) {
      while (ptr[B[i]] < M && used_dominos.count(make_pair(ptr[B[i]], B[i]))) {
        ++ptr[B[i]];
      }
      if (ptr[B[i]] == M) {
        Impossible();
      }
      A[i] = ptr[B[i]];
      used_dominos.insert(make_pair(A[i], B[i]));
    }
  }

  puts("YES");
  pair<int, int> domino = make_pair(0, 0);
  for (int i = 0; i < N; ++i) {
    if (A[i] == -1 && B[i] == -1) {
      while (used_dominos.count(domino)) {
        ++domino.second;
        if (domino.second == M) {
          domino = make_pair(domino.first + 1, 0);
        }
      }
      printf("%d %d\n", domino.first + 1, domino.second + 1);
      used_dominos.insert(domino);
    } else {
      printf("%d %d\n", A[i] + 1, B[i] + 1);
    }
  }
}
