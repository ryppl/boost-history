#include <iostream>
#include <iterator>
#include <algorithm>
#include <mtl/matrix.hpp>
#include <mtl/add_test.hpp>

int
main()
{
  const int n = 3;

  double A = 2.0, B = 3.0, D = 4.0;
  mtl::matrix<double>::type a(n, n, A), b(n, n, B), c(n, n), d(n, n, D);

  add_test(a, b, c, d);

  return 0;
}
