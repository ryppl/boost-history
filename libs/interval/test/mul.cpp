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

bool test_square(double al, double au) {
  I a(al, au);
  I b = square(a);
  I c = a * a;
  return b.upper() == c.upper() &&
         (b.lower() == c.lower() || (c.lower() <= 0 && b.lower() == 0));
}

bool test_sqrt(double al, double au) {
  I a(al, au);
  I b = square(sqrt(a));
  return subset(abs(a), b);
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

  BOOST_TEST(test_square(2, 3));
  BOOST_TEST(test_square(-2, 3));
  BOOST_TEST(test_square(-3, 2));

  BOOST_TEST(test_sqrt(2, 3));
  BOOST_TEST(test_sqrt(5, 7));
  BOOST_TEST(test_sqrt(-1, 2));

  return 0;
}
