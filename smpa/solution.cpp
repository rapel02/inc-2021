// O(N + M(log M + log MOD) + log K)

#include <bits/stdc++.h>
using namespace std;

const int kMod = 998244353;
const int kMaxM = 1 << 18;

int power(int x, int y, int mod=kMod) {
  int ret = 1;
  for (; y; y >>= 1) {
    if (y & 1) ret = 1LL * ret * x % mod;
    x = 1LL * x * x % mod;
  }
  return ret;
}

namespace FFT {

const int kMaxLogN = 21;
const int kMaxN = 1 << kMaxLogN;
const int kPrimitiveRoot = 3;

int root[kMaxN];

void init_fft() {
  const int kUnity = power(kPrimitiveRoot, (kMod-1) >> kMaxLogN);
  root[0] = 1;
  for (int i = 1; i < kMaxN; i++) {
    root[i] = 1LL * kUnity * root[i-1] % kMod;
  }
}

void fft(int n, int a[], bool invert) {
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j >= bit; bit >>= 1) j -= bit;
    j += bit;
    if (i < j) swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    int wlen = (invert ? root[kMaxN - kMaxN/len] : root[kMaxN/len]);
    for (int i=0; i<n; i+=len) {
      int w = 1;
      for (int j=0; j<len>>1; j++) {
        int u = a[i+j];
        int v = 1LL * a[i+j + len/2] * w % kMod;

        a[i+j] = (u + v) % kMod;
        a[i+j + len/2] = (u - v + kMod) % kMod;

        w = 1LL * w * wlen % kMod;
      }
    }
  }

  if (invert) {
    int inv = power(n, kMod-2);
    for (int i=0; i<n; i++) a[i] = 1LL * a[i] * inv % kMod;
  }
}

}

int n, m, k;
int p[kMaxM];

int main() {
  scanf("%d %d %d", &n, &m, &k);
  for (int i = 0; i < n; ++i) {
    int a;
    scanf("%d", &a);
    ++p[a];
  }

  FFT::init_fft();
  FFT::fft(m, p, false);
  for (int i = 0; i < m; ++i) p[i] = power(p[i], power(2, k, kMod-1));
  FFT::fft(m, p, true);

  int sum = 0;
  for (int i = 1; i < m; ++i) sum = (1LL * p[i] * i + sum) % kMod;
  printf("%d\n", sum);
  return 0;
}
