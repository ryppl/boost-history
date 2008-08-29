//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_DISABLE_WIN32

#include <vector>
#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

void test_constructors() {
  // zero:
  {
    // default:
    boost::math::tools::polynomial<int> poly1;
    // iterator + size:
    int p2[] = {0};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int p3[] = {0, 0};
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));
    int p4[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    boost::math::tools::polynomial<int> poly4(p4, ARRAY_SIZE(p4));
    boost::math::tools::polynomial<int> poly5(p4, 0);	// strage but works
    // two iterators:
    int p6[] = {0, 0, 0, 0, 0, 0};
    boost::math::tools::polynomial<int> poly6(p6, p6 + ARRAY_SIZE(p6));
    boost::math::tools::polynomial<int> poly7(p6, p6);	// strange but works
    // value:
    boost::math::tools::polynomial<int> poly8(0);
    // std::vector:
    std::vector<int> p9;
    boost::math::tools::polynomial<int> poly9(p9);
    std::vector<int> p10(1);
    boost::math::tools::polynomial<int> poly10(p10);
    std::vector<int> p11(47);
    boost::math::tools::polynomial<int> poly11(p11);
    // copy:
    boost::math::tools::polynomial<int> poly12(poly1);

    BOOST_CHECK_EQUAL(poly1, poly2);
    BOOST_CHECK_EQUAL(poly2, poly3);
    BOOST_CHECK_EQUAL(poly3, poly4);
    BOOST_CHECK_EQUAL(poly4, poly5);
    BOOST_CHECK_EQUAL(poly5, poly6);
    BOOST_CHECK_EQUAL(poly6, poly7);
    BOOST_CHECK_EQUAL(poly7, poly8);
    BOOST_CHECK_EQUAL(poly8, poly9);
    BOOST_CHECK_EQUAL(poly9, poly10);
    BOOST_CHECK_EQUAL(poly10, poly11);
    BOOST_CHECK_EQUAL(poly11, poly12);
  }

  // normalizing leading zeros:
  {
    int p1[] = {4, 3, 2, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {4, 3, 2, 1, 0};
    boost::math::tools::polynomial<int> poly2(p2, p2 + ARRAY_SIZE(p2));	// two iterators
    int p3[] = {4, 3, 2, 1, 0, 0, 0, 0, 0};
    boost::math::tools::polynomial<int> poly3(p3, ARRAY_SIZE(p3));	// iterator + size

    BOOST_CHECK_EQUAL(poly1, poly2);
    BOOST_CHECK_EQUAL(poly2, poly3);
  }
  {
    int p1[] = {47};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {47, 0, 0, 0, 0, 0, 0, 0};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    boost::math::tools::polynomial<int> poly3(47);

    BOOST_CHECK_EQUAL(poly1, poly2);
    BOOST_CHECK_EQUAL(poly2, poly3);
  }
}

void test_interpolation() {
  {
    int x[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int y[] = {9, 45, 1013, 14757, 116505, 610349, 2418645, 7846533, 21913097};
    // w(0) = 9, w(1) = 45 etc.
    int res[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {-4, -3, -2, -1, 0, 1, 2, 3, 4};
    int y[] = {65536, 6561, 256, 1, 0, 1, 256, 6561, 65536};
    // w(-4) = 65536, w(-3) = 6561 etc.
    int res[] = {0, 0, 0, 0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {5, -7, -6, 0};
    int y[] = {1, -23, -54, -954};
    // w(5) = 1, w(-7) = -23 etc.
    int res[] = {-954, -84, 35, 4};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {45, -113, 0, 4, -58, 1, 98, 47723, -3, -243854, 238, 99, -4949};
    int y[] = {45, -113, 0, 4, -58, 1, 98, 47723, -3, -243854, 238, 99, -4949};
    int res[] = {0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {8, -15, 49, 112, -88, 1, 539, -449};
    int y[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {8, -15, 49, 112, -88, 1, 539, -449};
    int y[] = {5, 5, 5, 5, 5, 5, 5, 5};
    int res[] = {5};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
  {
    int x[] = {0, -1, 1, -2, 2, -3, 3};
    int y[] = {1, 4, 4, 85, 85, 820, 820};
    int res[] = {1, 0, 1, 0, 1, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));

    // constructor:
    boost::math::tools::polynomial<int> poly1(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly1, result);

    // nested function:
    boost::math::tools::polynomial<int> poly2; // default now
    poly2.interpolate(x, x + ARRAY_SIZE(x), y);
    BOOST_CHECK_EQUAL(poly2, result);
  }
}

int test_main(int argc, char* argv[]) {
  test_constructors();
  test_interpolation();
  return 0;
}

