#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include <boost/interval.hpp>

#define PI 3.14159265358979323846

typedef boost::interval<int>    I_i;
typedef boost::interval<float>  I_f;
typedef boost::interval<double> I_d;

int test_main(int, char *[]) {
  BOOST_TEST(in((int)PI,    I_i::pi()));
  BOOST_TEST(in((float)PI,  I_f::pi()));
  BOOST_TEST(in((double)PI, I_d::pi()));
  BOOST_TEST(subset(I_i::pi(), succ(I_i(PI))));
  BOOST_TEST(subset(I_f::pi(), succ(I_f(PI))));
  BOOST_TEST(subset(I_d::pi(), succ(I_d(PI))));
  return 0;
}
