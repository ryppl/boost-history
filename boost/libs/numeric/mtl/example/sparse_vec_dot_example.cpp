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

  mtl::sparse_pair_vector<double, row>::type x(n), r(n), s(n);
  mtl::sparse_pair_vector<double, column>::type y(n), w(n), z(n);

  std::set<int> xi, yi, zi, wi;
  for (int i = 0; i < n/3; ++i) {
    xi.insert(rand() % n);
    yi.insert(rand() % n);
    zi.insert(rand() % n);
    wi.insert(rand() % n);
  }
  typedef mtl::sparse_pair_vector<double>::type::value_type Pair;
  std::set<int>::iterator j;
  for(j = xi.begin(); j != xi.end(); ++j)
    x.push_back(Pair(*j, 2.0));

  for(j = yi.begin(); j != yi.end(); ++j)
    y.push_back(Pair(*j, 1.0));

  for(j = zi.begin(); j != zi.end(); ++j)
    z.push_back(Pair(*j, 3.0));

  for(j = wi.begin(); j != wi.end(); ++j)
    w.push_back(Pair(*j, 4.0));

  mtl::vector<double,row>::type dx(n), dr(n), ds(n);
  mtl::vector<double,column>::type dy(n), dw(n), dz(n);

  dx = x;
  dr = r;
  ds = s;
  dy = y;
  dw = w;
  dz = z;
  
  dz = dy + dw;
  ds = dx + dr;

  d = x * y;
  eq = d == std::inner_product(dx.begin(), dx.end(), dy.begin(), 0.0);
  assert(eq);

  d = x * (y + w);
  eq = d == std::inner_product(dx.begin(), dx.end(), dz.begin(), 0.0);
  assert(eq);

  d = (x + r) * y;
  eq = d == std::inner_product(ds.begin(), ds.end(), dy.begin(), 0.0);
  assert(eq);

  d = (x + r) * (y + w);
  eq = d == std::inner_product(ds.begin(), ds.end(), dz.begin(), 0.0);
  assert(eq);

  return 0;
}

