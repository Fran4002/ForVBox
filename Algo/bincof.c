#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef long long ll;

const ll MOD = 1e9 + 7;

FILE * f;

ll fast_exp(ll n, ll k) {
  if (k == 1) return n;

  ll half = fast_exp(n, k / 2);

  ll ans = half * half % MOD;

  if (k % 2) {
    ans = ans * n % MOD;
  }

  return ans % MOD;
}

ll bin_coef(int n, int k) {
  ll fac = 1, div = 1, div2 = 1;
  
  for (int i = 2; i <= n; i++) fac = fac * i % MOD;
  for (int i = 2; i <= k; i++) div = div * i % MOD;
  for (int i = 2; i <= n - k; i++) div2 = div2 * i % MOD;
  
  ll tmp = fast_exp(div, MOD - 2) * fast_exp(div2, MOD - 2) % MOD;
  ll ans = fac * tmp % MOD;

  return ans;
}

ll dyv_coef(int n, int k) {
  if (k > n) return (ll)0;
  if (k == 0 || n == k) return (ll)1;
  return dyv_coef(n - 1, k - 1) + dyv_coef(n - 1, k);
}

ll coef(int n, int k) {
  ll ans;
  if (k > n) return 0;
  if (k > n / 2) k = n - k;

  ll *arr = calloc(sizeof(ll), n + 1);

  arr[0] = 1;

  for (int i = 1; i <= n; i++) {
    int lim = i < k ? i : k;
    ll last = arr[0];
    for (int j = 1; j <= lim; j++) {
      arr[j] = arr[j] + last;
      last = arr[j] - last;
    }
  }
  
  ans = arr[k];
  free(arr);
  
  return ans;
}

void test() {
  for (int i = 1; i <= 15; i++) {
    for (int j = 0; j <= i; j++) {
      ll dp = coef(i, j), dyv = dyv_coef(i, j);
      if (dp != dyv) {
        printf("ERROR at %d C %d\n", i, j);
        printf("dp: %lld\t dyv: %lld\n", dp, dyv);
        exit(1);
      }
    }
  }
  printf("OK\n");
}

void solve (ll func(int, int), int n, int k) {
  clock_t t_ini, t_end, t_elapsed;
  t_ini = clock();
  func(n, k);
  t_end = clock() - t_ini;
  t_elapsed = (double)t_end/CLOCKS_PER_SEC;
  fprintf(f, ",%f", (double)t_elapsed);
}

void test_k_const (int k) {
  int arr_n[] = {6, 9, 12, 15, 18};

  for (int i = 0; i < 5; i++) {
    fprintf(f, "%d,%d", arr_n[i], k);
    solve(dyv_coef, arr_n[i], k);
    solve(coef, arr_n[i], k);
    fprintf(f, "\n");
  }
}

void test_n_const (int n) {
  int arr_k[] = {0, 3, 7, 15, 22};

  for (int i = 0; i < sizeof(arr_k)/sizeof(arr_k[0]); i++) {
    fprintf(f, "%d,%d", n, arr_k[i]);
    solve(dyv_coef, n, arr_k[i]);
    solve(coef, n, arr_k[i]);
    fprintf(f, "\n");
  }
}

void bench () {
  for (int i = 5; i < 38; i += 4) {
    test_n_const(i);
  }
}

int main(void) {
  //coef(2, 1);
  f = fopen("bench.csv", "w");
  bench();
  return 0;
}