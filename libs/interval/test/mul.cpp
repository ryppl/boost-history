#include <boost/interval.hpp>
#include <boost/test/test_tools.hpp>

typedef boost::interval<double> I;

double min(double a, double b, double c, double d) {
  return std::min(std::min(a, b), std::min(c, d));
}

double max(double a, double b, double c, double d) {
  return std::max(std::max(a, b), std::max(c, d));
}

bool test_mul(double al, double au, double bl, double bu) {
  I a(al, au), b(bl, bu);
  I c = a * b;
  return c.lower() == min(al*bl, al*bu, au*bl, au*bu)
      && c.upper() == max(al*bl, al*bu, au*bl, au*bu);
}

bool test_div(double al, double au, double bl, double bu) {
  I a(al, au), b(bl, bu);
  I c = a / b;
  return c.lower() == min(al/bl, al/bu, au/bl, au/bu)
      && c.upper() == max(al/bl, al/bu, au/bl, au/bu);
}

int test_main(int, char*[]) {
  BOOST_TEST(test_mul(2, 3, 5, 7));
  BOOST_TEST(test_mul(2, 3, -5, 7));
  BOOST_TEST(test_mul(2, 3, -7, -5));
  BOOST_TEST(test_mul(-2, 3, 5, 7));
  BOOST_TEST(test_mul(-2, 3, -5, 7));
  BOOST_TEST(test_mul(-2, 3, -7, -5));
  BOOST_TEST(test_mul(-3, -2, 5, 7));
  BOOST_TEST(test_mul(-3, -2, -5, 7));
  BOOST_TEST(test_mul(-3, -2, -7, -5));

  BOOST_TEST(test_div(30, 42, 2, 3));
  BOOST_TEST(test_div(30, 42, -3, -2));
  BOOST_TEST(test_div(-30, 42, 2, 3));
  BOOST_TEST(test_div(-30, 42, -3, -2));
  BOOST_TEST(test_div(-42, -30, 2, 3));
  BOOST_TEST(test_div(-42, -30, -3, -2));

  return 0;
}
