#include <stdlib.h>
#include <time.h>
#include <set>
#include <mtl/vector.hpp>
#include <mtl/sparse_pair_vector.hpp>
#include <mtl/sd_add_test.hpp>

typedef mtl::vector<double>::type DVector;
typedef mtl::sparse_pair_vector<double>::type SVector;

int
main()
{
  srand(clock());

  const int n = 20;
  SVector x(n), w(n);
  DVector dx(n), y(n), z(n), dw(n);

  std::set<int> xi, yi, zi, wi;
  for (int i = 0; i < n/3; ++i) {
    xi.insert(rand() % n);
    yi.insert(rand() % n);
    zi.insert(rand() % n);
    wi.insert(rand() % n);
  }
  typedef SVector::value_type Pair;
  std::set<int>::iterator j;
  for(j = xi.begin(); j != xi.end(); ++j)
    x.push_back(Pair(*j, 2.0));

  for(j = yi.begin(); j != yi.end(); ++j)
    y[*j] = 1.0;

  for(j = zi.begin(); j != zi.end(); ++j)
    z[*j] = 3.0;

  for(j = wi.begin(); j != wi.end(); ++j)
    w.push_back(Pair(*j, 4.0));

  dx = x;
  dw = w;
  sd_add_test(x, y, z, w, dx, dw);

  return 0;
}

