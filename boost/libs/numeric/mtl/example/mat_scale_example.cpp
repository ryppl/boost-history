#include <iostream>
#include <iterator>
#include <algorithm>
#include <mtl/matrix.hpp>
#include <mtl/scale_test.hpp>

int
main()
{
  const int n = 3;

  double A = 2.0, B = 3.0, D = 4.0;
  double alpha = 5.0;
  mtl::matrix<double>::type a(n, n, A), b(n, n, B), c(n, n), d(n, n, D);

  scale_test(a, b, c, d, alpha);

  return 0;
}
