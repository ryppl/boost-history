#include <cassert>
#include <iterator>
#include <algorithm>
#include <mtl/matrix.hpp>
#include <mtl/vector.hpp>

int
main()
{
  int i, j;
  const int m = 4, n = 3;

  mtl::matrix<double, row>::type A(m, n), C(m, n);
  mtl::matrix<double, column>::type B(n, m), D(m, n);
  mtl::vector<double, column>::type x(n), y(m), s(n), p(m), u(m);
  mtl::vector<double, row>::type z(n), w(m), r(m), v(n);

  for (i = 0; i < m; ++i)
    for (j = 0; j < n; ++j) {
      A[i][j] = i * n + j;
      C[i][j] = i * n + j;
    }

  for (j = 0; j < n; ++j)
    for (i = 0; i < m; ++i) {
      B[j][i] = i * n + j;
      D[j][i] = i * n + j;
    }

  for (i = 0; i < m; ++i) {
    y[i] = i;
    p[i] = i;
    z[i] = i;
  }

  for (j = 0; j < n; ++j) {
    x[j] = j;
    s[j] = j;
    w[j] = j;
  }

  // =================================
  // Row-major matrix tests 

  y = A * x;
  for (i = 0; i < m; ++i)
    assert(y[i] == A[i] * x);

  y = A * x + p;
  for (i = 0; i < m; ++i)
    assert(y[i] == A[i] * x + p[i]);

  y = p + A * x;
  for (i = 0; i < m; ++i)
    assert(y[i] == A[i] * x + p[i]);

  y = A * (x + s);
  for (i = 0; i < m; ++i)
    assert(y[i] == A[i] * (x + s));

  y = (A + C) * x;
  for (i = 0; i < m; ++i)
    assert(y[i] == (A[i] + C[i]) * x);

  y = (A + C) * (x + s);
  for (i = 0; i < m; ++i)
    assert(y[i] == (A[i] + C[i]) * (x + s));

  // =================================
  // Column-major matrix tests 

  y = B * x;
  u = 0;
  for (j = 0; j < n; ++j)
    u += B[j] * x[j];
  assert(y == u);

  y = B * (x + s);
  u = 0;
  for (j = 0; j < n; ++j)
    u += B[j] * (x[j] + s[j]);
  assert(y == u);

  y = (B + D) * x;
  u = 0;
  for (j = 0; j < n; ++j)
    u += (B[j] + D[j]) * x[j];
  assert(y == u);

  y = (B + D) * (x + s);
  u = 0;
  for (j = 0; j < n; ++j)
    u += (B[j] + D[j]) * (x[j] + s[j]);
  assert(y == u);

  // =================================
  // Row-major, left-multiplication

  z = w * A;
  v = 0;
  for (i = 0; i < m; ++i)
    v += w[i] * A[i];
  assert(z == v);

  z = (w + r) * A;
  v = 0;
  for (i = 0; i < m; ++i)
    v += (w[i] + r[i]) * A[i];
  assert(z == v);

  z = w * (A + C);
  v = 0;
  for (i = 0; i < m; ++i)
    v += w[i] * (A[i] + C[i]);
  assert(z == v);

  z = (w + r) * (A + C);
  v = 0;
  for (i = 0; i < m; ++i)
    v += (w[i] + r[i]) * (A[i] + C[i]);
  assert(z == v);

  // =================================
  // Column-major, left-multiplication

  z = w * B;
  for (j = 0; j < n; ++j)
    assert(z[j] == w * B[j]);
  
  z = (w + r) * B;
  for (j = 0; j < n; ++j)
    assert(z[j] == (w + r) * B[j]);
  
  z = w * (B + D);
  for (j = 0; j < n; ++j)
    assert(z[j] == w * (B[j] + D[j]));
  
  z = (w + r) * (B + D);
  assert(z[j] == (w + r) * (B[j] + D[j]));
  
  return 0;
}


