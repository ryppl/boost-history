#include <boost/interval.hpp>
#include <boost/interval/io.hpp>
#include <iostream>

// I is an interval class, the polynom is a simple array
template<class I>
I horner(const I& x, const I p[], int n) {

  // initialize and restore the rounding mode
  typename I::traits_type::rounding rnd;

  // define the unprotected version of the interval type
  typedef typename boost::interval_lib::unprotect<I>::type R;

  const R& a = x;
  R y = p[n - 1];
  for(int i = n - 2; i >= 0; i--) {
    y = y * a + (const R&)(p[i]);
  }
  return y;

  // restore the rounding mode with the destruction of rnd
}

int main() {
  boost::interval<double> p[3] = { -1.0, 0, 1.0 };
  boost::interval<double> x = 1.0;
  std::cout << horner(x, p, 3) << std::endl;
  return 0;
}
