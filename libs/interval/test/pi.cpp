#include <boost/interval.hpp>
#include <boost/test/test_tools.hpp>

#define PI 3.14159265358979323846

typedef boost::interval<int>         I_i;
typedef boost::interval<float>       I_f;
typedef boost::interval<double>      I_d;
typedef boost::interval<long double> I_ld;

using boost::interval_lib::pi;
using boost::interval_lib::pi_1_2;
using boost::interval_lib::pi_2_1;

int test_main(int, char *[]) {
  I_i  pi_i  = pi<I_i>();
  I_f  pi_f  = pi<I_f>();
  I_d  pi_d  = pi<I_d>();
  I_ld pi_ld = pi<I_ld>();

  BOOST_TEST(in((int)   PI, pi_i));
  BOOST_TEST(in((float) PI, pi_f));
  BOOST_TEST(in((double)PI, pi_d));
  BOOST_TEST(subset(pi_i, widen(I_i((int)   PI), 1)));
  BOOST_TEST(subset(pi_f, widen(I_f((float) PI), std::numeric_limits<float> ::min())));
  BOOST_TEST(subset(pi_d, widen(I_d((double)PI), std::numeric_limits<double>::min())));

  // We can't test the following equalities for interval<int>.
  I_f pi_f_1_2 = pi_1_2<I_f>();
  I_f pi_f_2_1 = pi_2_1<I_f>();

  I_d pi_d_1_2 = pi_1_2<I_d>();
  I_d pi_d_2_1 = pi_2_1<I_d>();

  I_ld pi_ld_1_2 = pi_1_2<I_ld>();
  I_ld pi_ld_2_1 = pi_2_1<I_ld>();

  BOOST_TEST(equal(2.0f * pi_f_1_2, pi_f));
  BOOST_TEST(equal(2.0  * pi_d_1_2, pi_d));
  BOOST_TEST(equal(2.0l * pi_ld_1_2, pi_ld));

  BOOST_TEST(equal(2.0f * pi_f, pi_f_2_1));
  BOOST_TEST(equal(2.0  * pi_d, pi_d_2_1));
  BOOST_TEST(equal(2.0l * pi_ld, pi_ld_2_1));

  return 0;
}
