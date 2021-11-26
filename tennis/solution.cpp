#include <bits/stdc++.h>
using namespace std;

int main() {
  int K, S, N;
  scanf("%d %d %d", &K, &S, &N);
  if (K <= 2) {
    puts(N % 2 == 0 && N >= 2 * S ? "YES" : "NO");
  } else {
    if (N < (long long)S * K) {
      puts("NO");
    } else {
      if (S == 1) {
        puts(N <= 2 * K - 2 || N % 2 == 0 ? "YES" : "NO");
      } else {
        puts("YES");
      }
    }
  }
}
