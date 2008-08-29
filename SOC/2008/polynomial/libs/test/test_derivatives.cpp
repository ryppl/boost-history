//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

void test() {
  {
    int p1[] = {0};
    int p2[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
  }
  {
    int p1[] = {0, 0, 0, 0, 0, 0, 0, 1};
    int p2[] = {0, 0, 0, 0, 0, 0, 7};
    int p3[] = {0, 0, 0, 0, 0, 42};
    int p4[] = {0, 0, 0, 0, 210};
    int p5[] = {0, 0, 0, 840};
    int p6[] = {0, 0, 2520};
    int p7[] = {0, 5040};
    int p8[] = {5040};
    int p9[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
    BOOST_CHECK_EQUAL(poly2.derivative(), poly3);
    BOOST_CHECK_EQUAL(poly3.derivative(), poly4);
    BOOST_CHECK_EQUAL(poly4.derivative(), poly5);
    BOOST_CHECK_EQUAL(poly5.derivative(), poly6);
    BOOST_CHECK_EQUAL(poly6.derivative(), poly7);
    BOOST_CHECK_EQUAL(poly7.derivative(), poly8);
    BOOST_CHECK_EQUAL(poly8.derivative(), poly9);
  }
  {
    int p1[] = {0, 0, 0, 0, 0, 0, 0, -1};
    int p2[] = {0, 0, 0, 0, 0, 0, -7};
    int p3[] = {0, 0, 0, 0, 0, -42};
    int p4[] = {0, 0, 0, 0, -210};
    int p5[] = {0, 0, 0, -840};
    int p6[] = {0, 0, -2520};
    int p7[] = {0, -5040};
    int p8[] = {-5040};
    int p9[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
    BOOST_CHECK_EQUAL(poly2.derivative(), poly3);
    BOOST_CHECK_EQUAL(poly3.derivative(), poly4);
    BOOST_CHECK_EQUAL(poly4.derivative(), poly5);
    BOOST_CHECK_EQUAL(poly5.derivative(), poly6);
    BOOST_CHECK_EQUAL(poly6.derivative(), poly7);
    BOOST_CHECK_EQUAL(poly7.derivative(), poly8);
    BOOST_CHECK_EQUAL(poly8.derivative(), poly9);
  }
  {
    int p1[] = {1, 1, 1, 1, 1, 1, 1, 1};
    int p2[] = {1, 2, 3, 4, 5, 6, 7};
    int p3[] = {2, 6, 12, 20, 30, 42};
    int p4[] = {6, 24, 60, 120, 210};
    int p5[] = {24, 120, 360, 840};
    int p6[] = {120, 720, 2520};
    int p7[] = {720, 5040};
    int p8[] = {5040};
    int p9[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
    BOOST_CHECK_EQUAL(poly2.derivative(), poly3);
    BOOST_CHECK_EQUAL(poly3.derivative(), poly4);
    BOOST_CHECK_EQUAL(poly4.derivative(), poly5);
    BOOST_CHECK_EQUAL(poly5.derivative(), poly6);
    BOOST_CHECK_EQUAL(poly6.derivative(), poly7);
    BOOST_CHECK_EQUAL(poly7.derivative(), poly8);
    BOOST_CHECK_EQUAL(poly8.derivative(), poly9);
  }
  {
    int p1[] = {-1, -1, -1, -1, -1, -1, -1, -1};
    int p2[] = {-1, -2, -3, -4, -5, -6, -7};
    int p3[] = {-2, -6, -12, -20, -30, -42};
    int p4[] = {-6, -24, -60, -120, -210};
    int p5[] = {-24, -120, -360, -840};
    int p6[] = {-120, -720, -2520};
    int p7[] = {-720, -5040};
    int p8[] = {-5040};
    int p9[] = {0};

    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p5, ARRAY_SIZE(p5));
    boost::math::tools::polynomial<int> poly6(p6, ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p7, ARRAY_SIZE(p7));
    boost::math::tools::polynomial<int> poly8(p8, ARRAY_SIZE(p8));
    boost::math::tools::polynomial<int> poly9(p9, ARRAY_SIZE(p9));

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
    BOOST_CHECK_EQUAL(poly2.derivative(), poly3);
    BOOST_CHECK_EQUAL(poly3.derivative(), poly4);
    BOOST_CHECK_EQUAL(poly4.derivative(), poly5);
    BOOST_CHECK_EQUAL(poly5.derivative(), poly6);
    BOOST_CHECK_EQUAL(poly6.derivative(), poly7);
    BOOST_CHECK_EQUAL(poly7.derivative(), poly8);
    BOOST_CHECK_EQUAL(poly8.derivative(), poly9);
  }
  {
    int p1[] = {655392, -80, 1, -213, 49, 0, -5, 0, 1};
    int p2[] = {-80, 2, -639, 196, 0, -30, 0, 8};
    int p3[] = {2, -1278, 588, 0, -150, 0, 56};
    int p4[] = {-1278, 1176, 0, -600, 0, 336};
    int p5[] = {1176, 0, -1800, 0, 1680};
    int p6[] = {0, -3600, 0, 6720};
    int p7[] = {-3600, 0, 20160};
    int p8[] = {0, 40320};
    int p9[] = {40320};
    int p10[] = {0};

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

    BOOST_CHECK_EQUAL(poly1.derivative(), poly2);
    BOOST_CHECK_EQUAL(poly2.derivative(), poly3);
    BOOST_CHECK_EQUAL(poly3.derivative(), poly4);
    BOOST_CHECK_EQUAL(poly4.derivative(), poly5);
    BOOST_CHECK_EQUAL(poly5.derivative(), poly6);
    BOOST_CHECK_EQUAL(poly6.derivative(), poly7);
    BOOST_CHECK_EQUAL(poly7.derivative(), poly8);
    BOOST_CHECK_EQUAL(poly8.derivative(), poly9);
    BOOST_CHECK_EQUAL(poly9.derivative(), poly10);
  }
}

int test_main(int argc, char* argv[]) {
  test();
  return 0;
}

