#include <boost/interval.hpp>
#include <boost/test/test_tools.hpp>

#define PI 3.14159265358979323846

typedef boost::interval<int>    I_i;
typedef boost::interval<float>  I_f;
typedef boost::interval<double> I_d;

using boost::interval_lib::pi;

int test_main(int, char *[]) {
  BOOST_TEST(in((int)   PI, pi<I_i>()));
  BOOST_TEST(in((float) PI, pi<I_f>()));
  BOOST_TEST(in((double)PI, pi<I_d>()));
  BOOST_TEST(subset(pi<I_i>(), widen(I_i((int)   PI), 1)));
  BOOST_TEST(subset(pi<I_f>(), widen(I_f((float) PI), std::numeric_limits<float> ::min())));
  BOOST_TEST(subset(pi<I_d>(), widen(I_d((double)PI), std::numeric_limits<double>::min())));
  return 0;
}
