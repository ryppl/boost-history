// Example program of how to use interval<> of rational<> numbers.
//
// Sylvain Pion, 2002.

#include <iostream>
#include <boost/interval.hpp>
#include <boost/rational.hpp>

using boost::interval;
using boost::rational;

typedef rational<double>  Rat;
typedef interval<Rat>     Interval;

int main() {
  Rat r(0), p(2, 3), q(3, 4);
  Interval z(0);
  Interval a(p, q);
  a += z;
  z *= z;
  a -= p;
  a /= q;
  std::cout << z << std::endl;
  std::cout << a << std::endl;
}
