#include <boost/interval.hpp>
#define BOOST_INCLUDE_MAIN
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
  BOOST_TEST(subset(pi<I_i>(), succ(I_i((int)   PI))));
  BOOST_TEST(subset(pi<I_f>(), succ(I_f((float) PI))));
  BOOST_TEST(subset(pi<I_d>(), succ(I_d((double)PI))));
  return 0;
}
