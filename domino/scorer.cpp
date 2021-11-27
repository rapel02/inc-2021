#include <bits/stdc++.h>
using namespace std;

int ac() {
  puts("AC");
  return 0;
}

int wa(string message) {
  fprintf(stderr, "%s\n", message.c_str());
  puts("WA");
  return 0;
}

const string kPossibleKey = "YES";
const string kImpossibleKey = "NO";

string tolower(string S) {
  std::transform(S.begin(), S.end(), S.begin(),
                 [] (unsigned char c) { return std::tolower(c); });
  return S;
}

int main(int argc, char *argv[])
{
  FILE *in = fopen(argv[1], "r");
  FILE *out = fopen(argv[2], "r");  // Judge's
  FILE *con = fopen(argv[3], "r");

  int N, M;
  fscanf(in, "%d %d", &N, &M);
  vector<int> A(N), B(N);
  for (int i = 0; i < N; ++i) {
    fscanf(in, "%d %d", &A[i], &B[i]);
  }

  char buffer[1000000];
  
  fscanf(out, "%s", buffer);
  string judge = buffer;
  if (fscanf(con, "%s", buffer) == EOF) {
    return wa("Unexpected EOF.");
  }
  string contestant = buffer;

  if (tolower(judge) == tolower(kImpossibleKey)) {
    if (tolower(contestant) != tolower(kImpossibleKey)) {
      return wa("Judge answer is `impossible`, contestant answer is not.");
    }
  } else {
    if (tolower(contestant) != tolower(kPossibleKey)) {
      return wa("Judge answer is `possible`, contestant answer is not."); 
    }
    set<pair<int, int>> used_dominos;
    for (int i = 0; i < N; ++i) {
      int a, b;
      if (fscanf(con, "%d %d", &a, &b) != 2) {
        return wa("Unexpected EOF.");
      }
      if (a < 1 || a > M || b < 1 || b > M) {
        return wa("Domino index out of bound.");
      }
      if ((A[i] != -1 && a != A[i]) || (B[i] != -1 && b != B[i])) {
        return wa("Domino does not match.");
      }
      if (used_dominos.count(make_pair(a, b))) {
        return wa("Duplicate domino."); 
      }
      used_dominos.insert(make_pair(a, b));
    }
  }

  fscanf(con, "\n");
  char dummy;
  if (fscanf(con, "%c", &dummy) == 1) {
    return wa("Expected EOF.");
  }

  return ac();
}
