//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

void test_plus() {
  {
    int p[] = {0};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(+poly, result);
  }
  {
    int p[] = {7};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {7};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(+poly, result);
  }
  {
    int p[] = {452, 93248, 237, -2392, -3432};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {452, 93248, 237, -2392, -3432};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(+poly, result);
  }
}

void test_minus() {
  {
    int p[] = {0};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(-poly, result);
  }
  {
    int p[] = {7};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {-7};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(-poly, result);
  }
  {
    int p[] = {452, 93248, 237, -2392, -3432};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int res[] = {-452, -93248, -237, 2392, 3432};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(-poly, result);
  }
}

void test_addition() {
  // same degrees:
  {
    int p1[] = {0};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {1, 2, 3, 4, 5, 6, 7};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {3, 4, 5, 6, 7, 8, 9};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {4, 6, 8, 10, 12, 14, 16};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {231463, -703, -9450, 190};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {403, 0, -289455, -8772};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {231866, -703, -298905, -8582};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {4, -5, 3, 10};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-7, 5, 0, -10};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-3, 0, 3};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {4325, -23427, -234827, 493, 1, -3442737, 3, -7347, 2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-4325, 23427, 234827, -493, -1, 3442737, -3, 7347, -2};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }

  // different degrees:
  {
    int p1[] = {-14, 7, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {47, -7, 45, -113, 2};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {33, 0, 46, -113, 2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {0, 9773, -14857, 48274, -472438, 10};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-403};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-403, 9773, -14857, 48274, -472438, 10};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }
  {
    int p1[] = {7, -113, 67, 539, 4922};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-7, 113, -67, -539, -4922, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 + poly2, result);
    BOOST_CHECK_EQUAL(poly2 + poly1, result);
  }

  // polynomial vs value
  {
    int p[] = {0};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly + val, result);
    BOOST_CHECK_EQUAL(val + poly, result);
  }
  {
    int p[] = {5};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {5};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly + val, result);
    BOOST_CHECK_EQUAL(val + poly, result);
  }
  {
    int p[] = {8};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = -7;
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly + val, result);
    BOOST_CHECK_EQUAL(val + poly, result);
  }
  {
    int p[] = {113, 48347, -3494278, 0, 382, 0, -37273, 219};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {113, 48347, -3494278, 0, 382, 0, -37273, 219};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly + val, result);
    BOOST_CHECK_EQUAL(val + poly, result);
  }
  {
    int p[] = {-24, 48392, -43};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = -2839;
    int res[] = {-2863, 48392, -43};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly + val, result);
    BOOST_CHECK_EQUAL(val + poly, result);
  }
}

void test_substraction() {
  // same degrees:
  {
    int p1[] = {0};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }
  {
    int p1[] = {1, 2, 3, 4, 5, 6, 7};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {3, 4, 5, 6, 7, 8, 9};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-2, -2, -2, -2, -2, -2, -2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }
  {
    int p1[] = {231463, -703, -9450, 190};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {403, 0, -289455, -8772};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {231060, -703, 280005, 8962};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }
  {
    int p1[] = {4325, -23427, -234827, 493, 1, -3442737, 3, -7347, 2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {4325, -23427, -234827, 493, 1, -3442737, 3, -7347, 2};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }

  // different degrees:
  {
    int p1[] = {-14, 7, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {47, -7, 45, -113, 2};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-61, 14, -44, 113, -2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }
  {
    int p1[] = {0, 9773, -14857, 48274, -472438, 10};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-403};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {403, 9773, -14857, 48274, -472438, 10};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }
  {
    int p1[] = {7, -113, 67, 539, 4922};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {7, -113, 67, 539, 4922, -1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 - poly2, result);
    BOOST_CHECK_EQUAL(poly2 - poly1, -result);
  }

  // polynomial vs value
  {
    int p[] = {0};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly - val, result);
    BOOST_CHECK_EQUAL(val - poly, result);
  }
  {
    int p[] = {5};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {5};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly - val, result);
    BOOST_CHECK_EQUAL(val - poly, -result);
  }
  {
    int p[] = {8};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = -7;
    int res[] = {15};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly - val, result);
    BOOST_CHECK_EQUAL(val - poly, -result);
  }
  {
    int p[] = {113, 48347, -3494278, 0, 382, 0, -37273, 219};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {113, 48347, -3494278, 0, 382, 0, -37273, 219};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly - val, result);
    BOOST_CHECK_EQUAL(val - poly, -result);
  }
  {
    int p[] = {-24, 48392, -43};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = -2839;
    int res[] = {2815, 48392, -43};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly - val, result);
    BOOST_CHECK_EQUAL(val - poly, -result);
  }
}

void test_multiplication() {
  // same degrees:
  {
    int p1[] = {0};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {0};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {5};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-2391};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-2391};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 2, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-1, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {-1, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, -2, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 0, 0, 0, -29};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -29};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1, 1, 1, 1, 1, 1, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1, 1, 1, 1, 1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {34, -12, 10, 0, 3, -8, 0, -2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-2, 44, 0, -1, 8, 31, -21, 50};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-68, 1520, -548, 406, 278, 1096, -1358, 2263, -866, 529, -309, 302, -462, 42, -100};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }

  // different degrees:
  {
    int p1[] = {0};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {3, 4, 5, 6, 7, 8, 9};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {403, 0, -289455, -8772};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {403, 0, -289455, -8772};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {-2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {12, -201, 0, 44, -1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-24, 402, 0, -88, 2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {-14, 7, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {47, -7, 45, -113, 2};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-658, 427, -632, 1890, -774, -99, 2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }
  {
    int p1[] = {-23, -41, -19, -89, -43, -55, -94, -29, -12, -42, -66, -49};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-71, -10, -59, -44, -78, -64, -39, -21, -41, -84, -74, -38, -20, -99, -46, -32, -10, -27};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1633, 3141, 3116, 9940, 8662, 15092, 18680, 18376, 20703, 23474, 27413, 27624, 26508, 29216, 31604, 31008, 30653, 22835, 22701, 26312, 22624, 14215, 11245, 12658, 10134, 5110, 3362, 2272, 1323};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 * poly2, result);
    BOOST_CHECK_EQUAL(poly2 * poly1, result);
  }

  // polynomial vs value
  {
    int p[] = {0};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly * val, result);
    BOOST_CHECK_EQUAL(val * poly, result);
  }
  {
    int p[] = {5};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 0;
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly * val, result);
    BOOST_CHECK_EQUAL(val * poly, result);
  }
  {
    int p[] = {8};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = -7;
    int res[] = {-56};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly * val, result);
    BOOST_CHECK_EQUAL(val * poly, result);
  }
  {
    int p[] = {113, 48347, -3494278, 0, 382, 0, -37273, 219};
    boost::math::tools::polynomial<int> poly(p, ARRAY_SIZE(p));
    int val = 301;
    int res[] = {34013, 14552447, -1051777678, 0, 114982, 0, -11219173, 65919};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly * val, result);
    BOOST_CHECK_EQUAL(val * poly, result);
  }
}

void test_division() {
  // some simple tests...
  {
    int p1[] = {1, 2, 3};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 2, 3};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {1, 2, 3};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-1, -2, -3};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {7, -21, 70, -14, 0, -42, 0, 7};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {7};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, -3, 10, -2, 0, -6, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {265, -45827, 2384, 0, 249, -4, 2838, 29, -652344, 237, 572, -238157, 238, -12487, 234789, -21347};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {265, -45827, 2384, 0, 249, -4, 2838, 29, -652344, 237, 572, -238157, 238, -12487, 234789, -21347};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {-2, 4, 0, 2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-1, 2, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 0, 0, 0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 0, 0, 0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {1, 2, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {-1, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-1, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  // ...and some harder...
  {
    int p1[] = {4, 13, 28, 27, 18};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 2, 3};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {4, 5, 6};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {4, 13, 28, 27, 18};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {4, 5, 6};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 2, 3};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {5, 0, -10, -10, 5, 10, 0, 0, 5, 0, -10, -10, 5, 10, 5, 0, -5, -10, -5, 0, 5, 10, 5, 0, -5, -10, -5, 0, 5, 10, 5, -10, -10, 0, 5, 0, 0, 10, 5, -10, -10, 0, 5};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {5, 0, -5, 0, 5, 0, -5, 0, 5, 0, -5, 0, 5};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {5, 0, -10, -10, 5, 10, 0, 0, 5, 0, -10, -10, 5, 10, 5, 0, -5, -10, -5, 0, 5, 10, 5, 0, -5, -10, -5, 0, 5, 10, 5, -10, -10, 0, 5, 0, 0, 10, 5, -10, -10, 0, 5};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1, 2, 1, 0, -1, -2, -1, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {5, 0, -5, 0, 5, 0, -5, 0, 5, 0, -5, 0, 5};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 43, -53, 43, 20677, -36639, 15298, -14821, -198396, 592951, -296514, 3468169, -2050653, 312406, -2318916, 656030, 1764};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1, 0, 1, 482, -247, 28, -342, 98};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {43, -53, 0, 4, -472, 999, -87, 6757, 18};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, 43, -53, 43, 20677, -36639, 15298, -14821, -198396, 592951, -296514, 3468169, -2050653, 312406, -2318916, 656030, 1764};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {43, -53, 0, 4, -472, 999, -87, 6757, 18};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 1, 0, 1, 482, -247, 28, -342, 98};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  // ...and now with rest (which is lost)
  {
    int p1[] = {3, 53, -11, 0, 49, 11};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-11, 0, 49, 11};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {2, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 0, 0, 0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {1625, -940, -275, 734, 6942, -240, -4502, 188};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {14, -7, 0, 5, 94};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {74, 0, -48, 2};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
  {
    int p1[] = {0, -432, 1983, 14, 3252, -9085, 1431, -5055, 9609, -2421, 2710, 2708, 1015, -4503};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {48, -23, 0, -98, 37, -1, 0, 29, 57};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, -9, 37, 18, 58, -79};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 / poly2, result);
  }
}

void test_modulo() {
  // rest = 0:
  {
    int p1[] = {0, 8};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 4};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {1, 2, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {-1, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {-1, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-1, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {4, 0, -12, 6, -2};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-2, 0, 6, -3, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {4, 13, 28, 27, 18};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 2, 3};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {4, 13, 28, 27, 18};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {4, 5, 6};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {0, 0, 0, 0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {0, 0, 0, 0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, -48, 1, -49, 999, 3791};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {0, 43, -53, 43, 20677, -36639, 15298, -14821, -198396, 592951, -296514, 3468169, -2050653, 312406, -2318916, 656030, 1764};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {43, -53, 0, 4, -472, 999, -87, 6757, 18};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {0, 43, -53, 43, 20677, -36639, 15298, -14821, -198396, 592951, -296514, 3468169, -2050653, 312406, -2318916, 656030, 1764};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1, 0, 1, 482, -247, 28, -342, 98};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  // rest != 0:
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {1, 2, 3, 4, 5, 6, 7};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 2, 3, 4, 5, 6, 7, 8};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 2, 3, 4, 5, 6, 7};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {2, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {1625, -940, -275, 734, 6942, -240, -4502, 188};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {14, -7, 0, 5, 94};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {589, -422, 397};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
  {
    int p1[] = {0, -432, 1983, 14, 3252, -9085, 1431, -5055, 9609, -2421, 2710, 2708, 1015, -4503};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, -9, 37, 18, 58, -79};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(poly1 % poly2, result);
  }
}

void test_gcd() {
  {
    int p1[] = {1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {47};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {94};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {47};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, 4};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, -8};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 4};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, -4};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 8};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 4};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, -4};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, -8};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 4};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, 0, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, -11, 5, -23, 7, 9, -1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {-5, 2, 8, -3, -3, 0, 1, 0, 1};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {21, -9, -4, 0, 5, 0, 3};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
    poly1 *= 3;
    poly2 *= 3;
    result *= 3;
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
  {
    int p1[] = {7, -10, 10, -11, 18, -7, -27, 10, 0, 4};
    boost::math::tools::polynomial<int> poly1(p1, ARRAY_SIZE(p1));
    int p2[] = {-70, 30, 0, 3, 3, 0, 1};
    boost::math::tools::polynomial<int> poly2(p2, ARRAY_SIZE(p2));
    int res[] = {-7, 3, 0, 1};
    boost::math::tools::polynomial<int> result(res, ARRAY_SIZE(res));
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
    poly1 *= -37;
    poly2 *= -37;
    result *= 37;
    BOOST_CHECK_EQUAL(gcd(poly1, poly2), result);
    BOOST_CHECK_EQUAL(gcd(poly2, poly1), result);
  }
}

int test_main(int argc, char* argv[]) {
  test_plus();
  test_minus();
  test_addition();
  test_substraction();
  test_multiplication();
  test_division();
  test_modulo();
  test_gcd();
  return 0;
}

