#include <iostream>
#include <iterator>
#include <algorithm>
#include <mtl/vector.hpp>

#include <mtl/scale_test.hpp>

typedef mtl::vector<double>::type Vector;

int
main()
{
  const int n = 3;
  double X = 2.0, Y = 3.0, W = 4.0;
  double alpha = 5.0;
  Vector x(n, X), y(n, Y), z(n), w(n, W);

  scale_test(x, y, z, w, alpha);

  return 0;
}

