#include <mtl/identity_matrix.hpp>
#include <mtl/vector.hpp>

// This doesn't work yet...

int
main()
{
  const int n = 5;
  mtl::vector<double>::type x(n, 2.0), y(n, 0);
  mtl::identity_matrix<double> I(n);
  y = I * x;

  for (int i = 0; i < n; ++i)
    assert(y[i] == 2.0);

  return 0;
}
