#include <boost/interval.hpp>
#include <boost/interval/io.hpp>
#include <vector>
#include <algorithm>
#include <utility>
#include <iostream>
#include <iomanip>

using namespace boost;
using namespace interval_lib;

typedef interval<double> I_aux;
typedef unprotect<I_aux>::type I;

I f(const I& x)
{ return x * (x - 1.) * (x - 2.) * (x - 3.) * (x - 4.); }
I f_diff(const I& x)
{ return (((5. * x - 40.) * x + 105.) * x - 100.) * x + 24.; }

std::vector<I> newton_raphson(const I& xs) {
  std::vector<I> l, res;
  I vf, vd, x, x1, x2;
  l.push_back(xs);
  while (!l.empty()) {
    x = l.back();
    l.pop_back();
    bool x2_used;
    double xx = median(x);
    vf = f(xx);
    vd = f_diff(x);
    x1 = xx - division_part1(vf, vd, x2_used);
    if (x2_used) x2 = xx - division_part2(vf, vd);
    if (overlap(x1, x)) x1 = intersect(x, x1);
    else if (x2_used) { x1 = x2; x2_used = false; }
    else continue;
    if (x2_used)
      if (overlap(x2, x)) x2 = intersect(x, x2);
      else x2_used = false;
    if (x2_used && width(x2) > width(x1)) std::swap(x1, x2);
    if (!in_zero(f(x1)))
      if (x2_used) { x1 = x2; x2_used = false; }
      else continue;
    if (width(x1) < 1e-10) res.push_back(x1);
    else if (width(x1) > 0.75 * width(x)) {
      std::pair<I, I> p = bisect(x);
      if (in_zero(f(p.first))) l.push_back(p.first);
      x2 = p.second;
      x2_used = true;
    } else l.push_back(x1);
    if (x2_used && in_zero(f(x2)))
      if (width(x2) < 1e-10) res.push_back(x1);
      else l.push_back(x2);
  }
  return res;
}

int main() {
  I_aux::traits_type::rounding rnd;
  std::vector<I> res = newton_raphson(I(-1, 5.1));
  std::cout << "Results: " << std::endl << std::setprecision(12);
  for(std::vector<I>::const_iterator i = res.begin(); i != res.end(); ++i)
    std::cout << "  " << *i << std::endl;
  std::cout << std::endl;
}
