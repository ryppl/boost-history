#include <assert.h>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <mtl/vector.hpp>

typedef mtl::vector<double>::type Vector;

int
main()
{
  int i;
  const int n = 3;
  double X = 2.0, Y = 3.0, W = 4.0;
  Vector x(n, X), y(n, Y), z(n), w(n, W);

  z = x + y;
  for (i = 0; i < n; ++i)
    assert(z[i] == X + Y);

  z = (x + y) + w;
  for (i = 0; i < n; ++i)
    assert(z[i] == (X + Y) + W);

  z = w + (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == W + (X + Y));

  z = (x + w) + (x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == (X + W) + (X + Y));

  z = -x;
  for (i = 0; i < n; ++i)
    assert(z[i] == -X);

  z = -(x + y);
  for (i = 0; i < n; ++i)
    assert(z[i] == -(X + Y));

  return 0;
}

