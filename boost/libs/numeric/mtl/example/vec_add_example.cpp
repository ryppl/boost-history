#include <mtl/vector.hpp>
#include <mtl/add_test.hpp>

typedef mtl::vector<double>::type Vector;

int
main()
{
  const int n = 10;
  Vector x(n, 2.0), y(n, 3.0), z(n), w(n, 4.0);

  add_test(x, y, z, w);

  return 0;
}

