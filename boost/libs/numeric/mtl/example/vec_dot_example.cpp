#include <iostream>
#include <iterator>
#include <numeric>
#include <mtl/vector.hpp>


bool equivalent(double x, double y)
{
  return std::abs(x - y) < 0.0000001;
}

int
main()
{
  typedef mtl::vector<double, row>::type RowVec;
  typedef mtl::vector<double, column>::type ColumnVec;

  double d, d_true;
  const int n = 3;
  double X = 2.0, Y = 3.0, W = 4.0, R = 2.4;
  RowVec x(n, X), r(n, R), s(n);
  ColumnVec y(n, Y), w(n, W), z(n);

  z = y + w;
  s = x + r;

  d = x * y;
  d_true = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = x * (y + w);
  d_true = std::inner_product(x.begin(), x.end(), z.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(x, y + w);
  d_true = std::inner_product(x.begin(), x.end(), z.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = (x + r) * y;
  d_true = std::inner_product(s.begin(), s.end(), y.begin(), 0.0) ;
  assert(equivalent(d, d_true));

  d = dot(x + r, y);
  d_true = std::inner_product(s.begin(), s.end(), y.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = (x + r) * (y + w);
  d_true = std::inner_product(s.begin(), s.end(), z.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(x + r, y + w);
  d_true = std::inner_product(s.begin(), s.end(), z.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(x, y);
  d_true = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(y, x);
  d_true = std::inner_product(y.begin(), y.end(), x.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(x, x);
  d_true = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
  assert(equivalent(d, d_true));

  d = dot(y, y);
  d_true = std::inner_product(y.begin(), y.end(), y.begin(), 0.0);
  assert(equivalent(d, d_true));

  return 0;
}

