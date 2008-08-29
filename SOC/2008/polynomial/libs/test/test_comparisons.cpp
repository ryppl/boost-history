//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_DISABLE_WIN32

#include <iterator>
#include <vector>
#include <boost/test/included/test_exec_monitor.hpp>
#include "../../boost/polynomial.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template<typename T>
struct test_entry {
  boost::math::tools::polynomial<T> poly1;
  boost::math::tools::polynomial<T> poly2;
  bool equal;
  test_entry(boost::math::tools::polynomial<T>& p1, boost::math::tools::polynomial<T>& p2, bool eq) {
    poly1 = p1;
    poly2 = p2;
    equal = eq;
  }
};

template<typename T>
void test_different_degrees() {
  std::vector<test_entry<T> > tests;

  T p1[] = {T(4.0)};
  boost::math::tools::polynomial<T> poly1(p1, ARRAY_SIZE(p1));
  T p2[] = {T(3.1), T(2.4)};
  boost::math::tools::polynomial<T> poly2(p2, ARRAY_SIZE(p2));
  T p3[] = {T(2.7), T(-12.4), T(4.0)};
  boost::math::tools::polynomial<T> poly3(p3, ARRAY_SIZE(p3));
  T p4[] = {T(-11.1), T(-2.0), T(-0.1), T(9.7)};
  boost::math::tools::polynomial<T> poly4(p4, ARRAY_SIZE(p4));
  T p5[] = {T(45.2), T(-8.6), T(-5.9), T(3.2), T(-2.1)};
  boost::math::tools::polynomial<T> poly5(p5, ARRAY_SIZE(p5));
  T p6[] = {T(45.2), T(-8.6), T(-5.9), T(3.2), T(-2.1), T(10.0)};
  boost::math::tools::polynomial<T> poly6(p6, ARRAY_SIZE(p6));
  tests.push_back(test_entry<T>(poly1, poly1, true));
  tests.push_back(test_entry<T>(poly1, poly2, false));
  tests.push_back(test_entry<T>(poly1, poly3, false));
  tests.push_back(test_entry<T>(poly1, poly4, false));
  tests.push_back(test_entry<T>(poly1, poly5, false));
  tests.push_back(test_entry<T>(poly1, poly6, false));
  tests.push_back(test_entry<T>(poly2, poly2, true));
  tests.push_back(test_entry<T>(poly2, poly3, false));
  tests.push_back(test_entry<T>(poly2, poly4, false));
  tests.push_back(test_entry<T>(poly2, poly5, false));
  tests.push_back(test_entry<T>(poly2, poly6, false));
  tests.push_back(test_entry<T>(poly3, poly3, true));
  tests.push_back(test_entry<T>(poly3, poly4, false));
  tests.push_back(test_entry<T>(poly3, poly5, false));
  tests.push_back(test_entry<T>(poly3, poly6, false));
  tests.push_back(test_entry<T>(poly4, poly4, true));
  tests.push_back(test_entry<T>(poly4, poly5, false));
  tests.push_back(test_entry<T>(poly4, poly6, false));
  tests.push_back(test_entry<T>(poly5, poly5, true));
  tests.push_back(test_entry<T>(poly5, poly6, false));
  tests.push_back(test_entry<T>(poly6, poly6, true));

  for(typename std::vector<test_entry<T> >::iterator it = tests.begin(); it != tests.end(); ++it) {
    BOOST_CHECK_EQUAL(it->poly1 == it->poly2, !(it->poly1 != it->poly2));
    BOOST_CHECK_EQUAL(it->poly1 == it->poly2, it->equal);
  }
}

template<typename T>
void test_same_degrees() {
  std::vector<test_entry<T> > tests;

  T p1[] = {T(4.0), T(3.1), T(-2.2), T(-8.8), T(2.0), T(-9.9)};
  boost::math::tools::polynomial<T> poly1(p1, ARRAY_SIZE(p1));
  T p2[] = {T(3.1), T(2.4), T(-9.9), T(0.0), T(3.0), T(12.1)};
  boost::math::tools::polynomial<T> poly2(p2, ARRAY_SIZE(p2));
  T p3[] = {T(2.7), T(-12.4), T(4.0), T(0.0), T(-12.2), T(-8.5)};
  boost::math::tools::polynomial<T> poly3(p3, ARRAY_SIZE(p3));
  T p4[] = {T(-11.1), T(-2.0), T(-0.1), T(9.7), T(-4.2), T(-1.0)};
  boost::math::tools::polynomial<T> poly4(p4, ARRAY_SIZE(p4));
  T p5[] = {T(45.2), T(-8.6), T(-5.9), T(3.2), T(-2.1), T(4.7)};
  boost::math::tools::polynomial<T> poly5(p5, ARRAY_SIZE(p5));
  T p6[] = {T(45.2), T(-8.6), T(-5.9), T(3.2), T(-2.1), T(10.0)};
  boost::math::tools::polynomial<T> poly6(p6, ARRAY_SIZE(p6));
  tests.push_back(test_entry<T>(poly1, poly1, true));
  tests.push_back(test_entry<T>(poly1, poly2, false));
  tests.push_back(test_entry<T>(poly1, poly3, false));
  tests.push_back(test_entry<T>(poly1, poly4, false));
  tests.push_back(test_entry<T>(poly1, poly5, false));
  tests.push_back(test_entry<T>(poly1, poly6, false));
  tests.push_back(test_entry<T>(poly2, poly2, true));
  tests.push_back(test_entry<T>(poly2, poly3, false));
  tests.push_back(test_entry<T>(poly2, poly4, false));
  tests.push_back(test_entry<T>(poly2, poly5, false));
  tests.push_back(test_entry<T>(poly2, poly6, false));
  tests.push_back(test_entry<T>(poly3, poly3, true));
  tests.push_back(test_entry<T>(poly3, poly4, false));
  tests.push_back(test_entry<T>(poly3, poly5, false));
  tests.push_back(test_entry<T>(poly3, poly6, false));
  tests.push_back(test_entry<T>(poly4, poly4, true));
  tests.push_back(test_entry<T>(poly4, poly5, false));
  tests.push_back(test_entry<T>(poly4, poly6, false));
  tests.push_back(test_entry<T>(poly5, poly5, true));
  tests.push_back(test_entry<T>(poly5, poly6, false));
  tests.push_back(test_entry<T>(poly6, poly6, true));

  for(typename std::vector<test_entry<T> >::iterator it = tests.begin(); it != tests.end(); ++it) {
    BOOST_CHECK_EQUAL(it->poly1 == it->poly2, !(it->poly1 != it->poly2));
    BOOST_CHECK_EQUAL(it->poly1 == it->poly2, it->equal);
  }
}

int test_main(int argc, char* argv[]) {
  test_different_degrees<int>();
  test_different_degrees<double>();
  test_same_degrees<int>();
  test_same_degrees<double>();
  return 0;
}

