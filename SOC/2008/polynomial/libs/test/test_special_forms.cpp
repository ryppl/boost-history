//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

void test_chebyshev_form() {
  int p0[] = {1};
  int p1[] = {0, 1};
  int p2[] = {-1, 0, 2};
  int p3[] = {0, -3, 0, 4};
  int p4[] = {1, 0, -8, 0, 8};
  int p5[] = {0, 5, 0, -20, 0, 16};
  int p6[] = {-1, 0, 18, 0, -48, 0, 32};
  int p7[] = {0, -7, 0, 56, 0, -112, 0, 64};
  int p8[] = {1, 0, -32, 0, 160, 0, -256, 0, 128};
  int p9[] = {0, 9, 0, -120, 0, 432, 0, -576, 0, 256};
  int p10[] = {-1, 0, 50, 0, -400, 0, 1120, 0, -1280, 0, 512};
  int p11[] = {0, -11, 0, 220, 0, -1232, 0, 2816, 0, -2816, 0, 1024};
  int p12[] = {1, 0, -72, 0, 840, 0, -3584, 0, 6912, 0, -6144, 0, 2048};
  int p13[] = {0, 13, 0, -364, 0, 2912, 0, -9984, 0, 16640, 0, -13312, 0, 4096};
  int p14[] = {-1, 0, 98, 0, -1568, 0, 9408, 0, -26880, 0, 39424, 0, -28672, 0, 8192};
  int p15[] = {0, -15, 0, 560, 0, -6048, 0, 28800, 0, -70400, 0, 92160, 0, -61440, 0, 16384};
  int p16[] = {1, 0, -128, 0, 2688, 0, -21504, 0, 84480, 0, -180224, 0, 212992, 0, -131072, 0, 32768};
  int p17[] = {0, 17, 0, -816, 0, 11424, 0, -71808, 0, 239360, 0, -452608, 0, 487424, 0, -278528, 0, 65536};
  int p18[] = {-1, 0, 162, 0, -4320, 0, 44352, 0, -228096, 0, 658944, 0, -1118208, 0, 1105920, 0, -589824, 0, 131072};
  int p19[] = {0, -19, 0, 1140, 0, -20064, 0, 160512, 0, -695552, 0, 1770496, 0, -2723840, 0, 2490368, 0, -1245184, 0, 262144};

  boost::math::tools::polynomial<int> t0(p0, ARRAY_SIZE(p0));
  boost::math::tools::polynomial<int> t1(p1, ARRAY_SIZE(p1));
  boost::math::tools::polynomial<int> t2(p2, ARRAY_SIZE(p2));
  boost::math::tools::polynomial<int> t3(p3, ARRAY_SIZE(p3));
  boost::math::tools::polynomial<int> t4(p4, ARRAY_SIZE(p4));
  boost::math::tools::polynomial<int> t5(p5, ARRAY_SIZE(p5));
  boost::math::tools::polynomial<int> t6(p6, ARRAY_SIZE(p6));
  boost::math::tools::polynomial<int> t7(p7, ARRAY_SIZE(p7));
  boost::math::tools::polynomial<int> t8(p8, ARRAY_SIZE(p8));
  boost::math::tools::polynomial<int> t9(p9, ARRAY_SIZE(p9));
  boost::math::tools::polynomial<int> t10(p10, ARRAY_SIZE(p10));
  boost::math::tools::polynomial<int> t11(p11, ARRAY_SIZE(p11));
  boost::math::tools::polynomial<int> t12(p12, ARRAY_SIZE(p12));
  boost::math::tools::polynomial<int> t13(p13, ARRAY_SIZE(p13));
  boost::math::tools::polynomial<int> t14(p14, ARRAY_SIZE(p14));
  boost::math::tools::polynomial<int> t15(p15, ARRAY_SIZE(p15));
  boost::math::tools::polynomial<int> t16(p16, ARRAY_SIZE(p16));
  boost::math::tools::polynomial<int> t17(p17, ARRAY_SIZE(p17));
  boost::math::tools::polynomial<int> t18(p18, ARRAY_SIZE(p18));
  boost::math::tools::polynomial<int> t19(p19, ARRAY_SIZE(p19));

  boost::math::tools::polynomial<int> poly;

  poly = boost::math::tools::chebyshev_form<int>(0);
  BOOST_CHECK_EQUAL(poly, t0);
  poly = boost::math::tools::chebyshev_form<int>(1);
  BOOST_CHECK_EQUAL(poly, t1);
  poly = boost::math::tools::chebyshev_form<int>(2);
  BOOST_CHECK_EQUAL(poly, t2);
  poly = boost::math::tools::chebyshev_form<int>(3);
  BOOST_CHECK_EQUAL(poly, t3);
  poly = boost::math::tools::chebyshev_form<int>(4);
  BOOST_CHECK_EQUAL(poly, t4);
  poly = boost::math::tools::chebyshev_form<int>(5);
  BOOST_CHECK_EQUAL(poly, t5);
  poly = boost::math::tools::chebyshev_form<int>(6);
  BOOST_CHECK_EQUAL(poly, t6);
  poly = boost::math::tools::chebyshev_form<int>(7);
  BOOST_CHECK_EQUAL(poly, t7);
  poly = boost::math::tools::chebyshev_form<int>(8);
  BOOST_CHECK_EQUAL(poly, t8);
  poly = boost::math::tools::chebyshev_form<int>(9);
  BOOST_CHECK_EQUAL(poly, t9);
  poly = boost::math::tools::chebyshev_form<int>(10);
  BOOST_CHECK_EQUAL(poly, t10);
  poly = boost::math::tools::chebyshev_form<int>(11);
  BOOST_CHECK_EQUAL(poly, t11);
  poly = boost::math::tools::chebyshev_form<int>(12);
  BOOST_CHECK_EQUAL(poly, t12);
  poly = boost::math::tools::chebyshev_form<int>(13);
  BOOST_CHECK_EQUAL(poly, t13);
  poly = boost::math::tools::chebyshev_form<int>(14);
  BOOST_CHECK_EQUAL(poly, t14);
  poly = boost::math::tools::chebyshev_form<int>(15);
  BOOST_CHECK_EQUAL(poly, t15);
  poly = boost::math::tools::chebyshev_form<int>(16);
  BOOST_CHECK_EQUAL(poly, t16);
  poly = boost::math::tools::chebyshev_form<int>(17);
  BOOST_CHECK_EQUAL(poly, t17);
  poly = boost::math::tools::chebyshev_form<int>(18);
  BOOST_CHECK_EQUAL(poly, t18);
  poly = boost::math::tools::chebyshev_form<int>(19);
  BOOST_CHECK_EQUAL(poly, t19);
}

void test_hermite_form() {
  int p0[] = {1};
  int p1[] = {0, 2};
  int p2[] = {-2, 0, 4};
  int p3[] = {0, -12, 0, 8};
  int p4[] = {12, 0, -48, 0, 16};
  int p5[] = {0, 120, 0, -160, 0, 32};
  int p6[] = {-120, 0, 720, 0, -480, 0, 64};
  int p7[] = {0, -1680, 0, 3360, 0, -1344, 0, 128};
  int p8[] = {1680, 0, -13440, 0, 13440, 0, -3584, 0, 256};
  int p9[] = {0, 30240, 0, -80640, 0, 48384, 0, -9216, 0, 512};
  int p10[] = {-30240, 0, 302400, 0, -403200, 0, 161280, 0, -23040, 0, 1024};
  int p11[] = {0, -665280, 0, 2217600, 0, -1774080, 0, 506880, 0, -56320, 0, 2048};
  int p12[] = {665280, 0, -7983360, 0, 13305600, 0, -7096320, 0, 1520640, 0, -135168, 0, 4096};
  int p13[] = {0, 17297280, 0, -69189120, 0, 69189120, 0, -26357760, 0, 4392960, 0, -319488, 0, 8192};

  boost::math::tools::polynomial<int> h0(p0, ARRAY_SIZE(p0));
  boost::math::tools::polynomial<int> h1(p1, ARRAY_SIZE(p1));
  boost::math::tools::polynomial<int> h2(p2, ARRAY_SIZE(p2));
  boost::math::tools::polynomial<int> h3(p3, ARRAY_SIZE(p3));
  boost::math::tools::polynomial<int> h4(p4, ARRAY_SIZE(p4));
  boost::math::tools::polynomial<int> h5(p5, ARRAY_SIZE(p5));
  boost::math::tools::polynomial<int> h6(p6, ARRAY_SIZE(p6));
  boost::math::tools::polynomial<int> h7(p7, ARRAY_SIZE(p7));
  boost::math::tools::polynomial<int> h8(p8, ARRAY_SIZE(p8));
  boost::math::tools::polynomial<int> h9(p9, ARRAY_SIZE(p9));
  boost::math::tools::polynomial<int> h10(p10, ARRAY_SIZE(p10));
  boost::math::tools::polynomial<int> h11(p11, ARRAY_SIZE(p11));
  boost::math::tools::polynomial<int> h12(p12, ARRAY_SIZE(p12));
  boost::math::tools::polynomial<int> h13(p13, ARRAY_SIZE(p13));

  boost::math::tools::polynomial<int> poly;

  poly = boost::math::tools::hermite_form<int>(0);
  BOOST_CHECK_EQUAL(poly, h0);
  poly = boost::math::tools::hermite_form<int>(1);
  BOOST_CHECK_EQUAL(poly, h1);
  poly = boost::math::tools::hermite_form<int>(2);
  BOOST_CHECK_EQUAL(poly, h2);
  poly = boost::math::tools::hermite_form<int>(3);
  BOOST_CHECK_EQUAL(poly, h3);
  poly = boost::math::tools::hermite_form<int>(4);
  BOOST_CHECK_EQUAL(poly, h4);
  poly = boost::math::tools::hermite_form<int>(5);
  BOOST_CHECK_EQUAL(poly, h5);
  poly = boost::math::tools::hermite_form<int>(6);
  BOOST_CHECK_EQUAL(poly, h6);
  poly = boost::math::tools::hermite_form<int>(7);
  BOOST_CHECK_EQUAL(poly, h7);
  poly = boost::math::tools::hermite_form<int>(8);
  BOOST_CHECK_EQUAL(poly, h8);
  poly = boost::math::tools::hermite_form<int>(9);
  BOOST_CHECK_EQUAL(poly, h9);
  poly = boost::math::tools::hermite_form<int>(10);
  BOOST_CHECK_EQUAL(poly, h10);
  poly = boost::math::tools::hermite_form<int>(11);
  BOOST_CHECK_EQUAL(poly, h11);
  poly = boost::math::tools::hermite_form<int>(12);
  BOOST_CHECK_EQUAL(poly, h12);
  poly = boost::math::tools::hermite_form<int>(13);
  BOOST_CHECK_EQUAL(poly, h13);
}

int test_main(int argc, char* argv[]) {
  test_chebyshev_form();
  test_hermite_form();
  return 0;
}

