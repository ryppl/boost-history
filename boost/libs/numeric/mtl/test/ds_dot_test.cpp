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
  srand(clock());
  double d;
  bool eq;
  const int n = 3;

  typedef mtl::vector<double, row>::type DRVec;
  DRVec x(n), r(n, 2.0), s(n);
  typedef mtl::sparse_pair_vector<double, column>::type SCVec;
  SCVec y(n), w(n), z(n);

  std::set<int> xi, ri, yi, zi, wi;
  for (int i = 0; i < n/3; ++i) {
    xi.insert(rand() % n);
    ri.insert(rand() % n);
    yi.insert(rand() % n);
    zi.insert(rand() % n);
    wi.insert(rand() % n);
  }
  typedef SCVec::value_type Pair;
  std::set<int>::iterator j;
  for(j = xi.begin(); j != xi.end(); ++j)
    x[*j] = 2.0;

  for(j = ri.begin(); j != ri.end(); ++j)
    r[*j] = 3.0;

  for(j = yi.begin(); j != yi.end(); ++j)
    y.push_back(Pair(*j, 1.0));

  for(j = zi.begin(); j != zi.end(); ++j)
    z.push_back(Pair(*j, 3.0));

  for(j = wi.begin(); j != wi.end(); ++j)
    w.push_back(Pair(*j, 4.0));

  typedef mtl::vector<double,column>::type DCVec;
  DCVec dy(n), dw(n), dz(n);

  dy = y;
  dw = w;
  dz = z;
  
  dz = dy + dw;
  s = x + r;

  d = x * y;
  eq = d == std::inner_product(x.begin(), x.end(), dy.begin(), 0.0);
  assert(eq);

  d = x * (y + w);
  eq = d == std::inner_product(x.begin(), x.end(), dz.begin(), 0.0);
  assert(eq);

  d = (x + r) * y;
  eq = d == std::inner_product(s.begin(), s.end(), dy.begin(), 0.0);
  assert(eq);

  d = (x + r) * (y + w);
  eq = d == std::inner_product(s.begin(), s.end(), dz.begin(), 0.0);
  assert(eq);

  return 0;
}

