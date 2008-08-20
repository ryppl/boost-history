#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

void test() {
  // with default coefficient for x^0:
  {
    int p1[] = {0};
    int p2[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));

    BOOST_CHECK_EQUAL(poly1.integral(), poly2);
  }
  {
    int p1[] = {5040};
    int p2[] = {0, 5040};
    int p3[] = {0, 0, 2520};
    int p4[] = {0, 0, 0, 840};
    int p5[] = {0, 0, 0, 0, 210};
    int p6[] = {0, 0, 0, 0, 0, 42};
    int p7[] = {0, 0, 0, 0, 0, 0, 7};
    int p8[] = {0, 0, 0, 0, 0, 0, 0, 1};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));

    BOOST_CHECK_EQUAL(poly1.integral(), poly2);
    BOOST_CHECK_EQUAL(poly2.integral(), poly3);
    BOOST_CHECK_EQUAL(poly3.integral(), poly4);
    BOOST_CHECK_EQUAL(poly4.integral(), poly5);
    BOOST_CHECK_EQUAL(poly5.integral(), poly6);
    BOOST_CHECK_EQUAL(poly6.integral(), poly7);
    BOOST_CHECK_EQUAL(poly7.integral(), poly8);
  }
  {
    int p1[] = {-5040};
    int p2[] = {0, -5040};
    int p3[] = {0, 0, -2520};
    int p4[] = {0, 0, 0, -840};
    int p5[] = {0, 0, 0, 0, -210};
    int p6[] = {0, 0, 0, 0, 0, -42};
    int p7[] = {0, 0, 0, 0, 0, 0, -7};
    int p8[] = {0, 0, 0, 0, 0, 0, 0, -1};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));

    BOOST_CHECK_EQUAL(poly1.integral(), poly2);
    BOOST_CHECK_EQUAL(poly2.integral(), poly3);
    BOOST_CHECK_EQUAL(poly3.integral(), poly4);
    BOOST_CHECK_EQUAL(poly4.integral(), poly5);
    BOOST_CHECK_EQUAL(poly5.integral(), poly6);
    BOOST_CHECK_EQUAL(poly6.integral(), poly7);
    BOOST_CHECK_EQUAL(poly7.integral(), poly8);
  }
  // with some coefficients for x^0:
  {
    int p1[] = {0};
    int p2[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));

    BOOST_CHECK_EQUAL(poly1.integral(0), poly2);
  }
  {
    int p1[] = {0};
    int p2[] = {47};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));

    BOOST_CHECK_EQUAL(poly1.integral(47), poly2);
  }
  {
    int p1[] = {0};
    int p2[] = {-113};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));

    BOOST_CHECK_EQUAL(poly1.integral(-113), poly2);
  }
  {
    int p1[] = {0};
    int p2[] = {5040};
    int p3[] = {720, 5040};
    int p4[] = {120, 720, 2520};
    int p5[] = {24, 120, 360, 840};
    int p6[] = {6, 24, 60, 120, 210};
    int p7[] = {2, 6, 12, 20, 30, 42};
    int p8[] = {1, 2, 3, 4, 5, 6, 7};
    int p9[] = {1, 1, 1, 1, 1, 1, 1, 1};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.integral(5040), poly2);
    BOOST_CHECK_EQUAL(poly2.integral(720), poly3);
    BOOST_CHECK_EQUAL(poly3.integral(120), poly4);
    BOOST_CHECK_EQUAL(poly4.integral(24), poly5);
    BOOST_CHECK_EQUAL(poly5.integral(6), poly6);
    BOOST_CHECK_EQUAL(poly6.integral(2), poly7);
    BOOST_CHECK_EQUAL(poly7.integral(1), poly8);
    BOOST_CHECK_EQUAL(poly8.integral(1), poly9);
  }
  {
    int p1[] = {0};
    int p2[] = {-5040};
    int p3[] = {-720, -5040};
    int p4[] = {-120, -720, -2520};
    int p5[] = {-24, -120, -360, -840};
    int p6[] = {-6, -24, -60, -120, -210};
    int p7[] = {-2, -6, -12, -20, -30, -42};
    int p8[] = {-1, -2, -3, -4, -5, -6, -7};
    int p9[] = {-1, -1, -1, -1, -1, -1, -1, -1};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.integral(-5040), poly2);
    BOOST_CHECK_EQUAL(poly2.integral(-720), poly3);
    BOOST_CHECK_EQUAL(poly3.integral(-120), poly4);
    BOOST_CHECK_EQUAL(poly4.integral(-24), poly5);
    BOOST_CHECK_EQUAL(poly5.integral(-6), poly6);
    BOOST_CHECK_EQUAL(poly6.integral(-2), poly7);
    BOOST_CHECK_EQUAL(poly7.integral(-1), poly8);
    BOOST_CHECK_EQUAL(poly8.integral(-1), poly9);
  }
  {
    int p1[] = {0};
    int p2[] = {40320};
    int p3[] = {0, 40320};
    int p4[] = {-3600, 0, 20160};
    int p5[] = {0, -3600, 0, 6720};
    int p6[] = {1176, 0, -1800, 0, 1680};
    int p7[] = {-1278, 1176, 0, -600, 0, 336};
    int p8[] = {2, -1278, 588, 0, -150, 0, 56};
    int p9[] = {-80, 2, -639, 196, 0, -30, 0, 8};
    int p10[] = {655392, -80, 1, -213, 49, 0, -5, 0, 1};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));
    boost::math::tools::polynomial<int> poly10(p10, ARRAY_SIZE(p10));

    BOOST_CHECK_EQUAL(poly1.integral(40320), poly2);
    BOOST_CHECK_EQUAL(poly2.integral(), poly3);
    BOOST_CHECK_EQUAL(poly3.integral(-3600), poly4);
    BOOST_CHECK_EQUAL(poly4.integral(), poly5);
    BOOST_CHECK_EQUAL(poly5.integral(1176), poly6);
    BOOST_CHECK_EQUAL(poly6.integral(-1278), poly7);
    BOOST_CHECK_EQUAL(poly7.integral(2), poly8);
    BOOST_CHECK_EQUAL(poly8.integral(-80), poly9);
    BOOST_CHECK_EQUAL(poly9.integral(655392), poly10);
  }
}

int test_main(int argc, char* argv[]) {
  test();
  return 0;
}

