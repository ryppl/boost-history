#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <mtl/vector.hpp>
#include <mtl/sparse_pair_vector.hpp>

int
main()
{
  typedef mtl::sparse_pair_vector<double, row>::type SRVec;
  typedef mtl::vector<double, column>::type DCVec;
  typedef mtl::vector<double, row>::type DRVec;

  srand(clock());
  double d;
  bool eq;
  const int n = 3;

  SRVec x(n), r(n), s(n);
  DCVec y(n), w(n), z(n);

  std::set<int> xi, ri, yi, zi, wi;
  for (int i = 0; i < n/3; ++i) {
    xi.insert(rand() % n);
    ri.insert(rand() % n);
    yi.insert(rand() % n);
    zi.insert(rand() % n);
    wi.insert(rand() % n);
  }
  typedef SRVec::value_type Pair;
  std::set<int>::iterator j;
  for(j = xi.begin(); j != xi.end(); ++j)
    x.push_back(Pair(*j, 2.0));

  for(j = ri.begin(); j != ri.end(); ++j)
    r.push_back(Pair(*j, 2.0));

  for(j = yi.begin(); j != yi.end(); ++j)
    y[*j] = 1.0;

  for(j = zi.begin(); j != zi.end(); ++j)
    z[*j] = 3.0;

  for(j = wi.begin(); j != wi.end(); ++j)
    w[*j] = 4.0;

  DRVec dx(n), dr(n), ds(n);

  dx = x;
  dr = r;
  ds = s;
  
  z = y + w;
  ds = dx + dr;

  d = x * y;
  eq = d == std::inner_product(dx.begin(), dx.end(), y.begin(), 0.0);
  assert(eq);

  d = x * (y + w);
  eq = d == std::inner_product(dx.begin(), dx.end(), z.begin(), 0.0);
  assert(eq);

  d = (x + r) * y;
  eq = d == std::inner_product(ds.begin(), ds.end(), y.begin(), 0.0);
  assert(eq);

  d = (x + r) * (y + w);
  eq = d == std::inner_product(ds.begin(), ds.end(), z.begin(), 0.0);
  assert(eq);

  return 0;
}

