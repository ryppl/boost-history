// Boost sweepline library robust_fpt_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#define BOOST_TEST_MODULE robust_fpt_test
#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>

#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline::detail;

typedef boost::mpl::list<double, mpf_class, mpt_wrapper<mpf_class, 8> > test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(empty_constructor_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a = rfpt_type();
    BOOST_CHECK_EQUAL(a.fpv() == static_cast<T>(0), true);
    BOOST_CHECK_EQUAL(a.re() == 0.0, true);
    BOOST_CHECK_EQUAL(a.ulp() == 0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(autorounded_constructor_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(10));
    BOOST_CHECK_EQUAL(a.fpv() == static_cast<T>(10), true);
    BOOST_CHECK_EQUAL(a.re() == 1.0, true);
    BOOST_CHECK_EQUAL(a.ulp() == 1.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(notrounded_constructor_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(10), false);
    BOOST_CHECK_EQUAL(a.fpv() == static_cast<T>(10), true);
    BOOST_CHECK_EQUAL(a.re() == 0.0, true);
    BOOST_CHECK_EQUAL(a.ulp() == 0.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(constructor_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(10), 3.0);
    BOOST_CHECK_EQUAL(a.fpv() == static_cast<T>(10), true);
    BOOST_CHECK_EQUAL(a.re() == 3.0, true);
    BOOST_CHECK_EQUAL(a.ulp() == 3.0, true);

    rfpt_type b(static_cast<T>(10), 2.75);
    BOOST_CHECK_EQUAL(b.fpv() == static_cast<T>(10), true);
    BOOST_CHECK_EQUAL(b.re() == 2.75, true);
    BOOST_CHECK_EQUAL(b.ulp() == 2.75, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(sum_test1, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(2), 5.0);
    rfpt_type b(static_cast<T>(3), 4.0);
    rfpt_type c = a + b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(5), true);
    BOOST_CHECK_EQUAL(c.re() == 6.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 6.0, true);

    c += b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(8), true);
    BOOST_CHECK_EQUAL(c.re() == 7.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 7.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(sum_test2, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(3), 2.0);
    rfpt_type b(static_cast<T>(-2), 3.0);
    rfpt_type c = a + b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(1), true);
    BOOST_CHECK_EQUAL(c.re() == 13.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 13.0, true);

    c += b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(-1), true);
    BOOST_CHECK_EQUAL(c.re() == 20.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 20.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_test1, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(2), 5.0);
    rfpt_type b(static_cast<T>(-3), 4.0);
    rfpt_type c = a - b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(5), true);
    BOOST_CHECK_EQUAL(c.re() == 6.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 6.0, true);

    c -= b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(8), true);
    BOOST_CHECK_EQUAL(c.re() == 7.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 7.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(minus_test2, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(3), 2.0);
    rfpt_type b(static_cast<T>(2), 3.0);
    rfpt_type c = a - b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(1), true);
    BOOST_CHECK_EQUAL(c.re() == 13.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 13.0, true);

    c -= b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(-1), true);
    BOOST_CHECK_EQUAL(c.re() == 20.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 20.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mult_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(2), 3.0);
    rfpt_type b(static_cast<T>(4));
    rfpt_type c = a * b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(8), true);
    BOOST_CHECK_EQUAL(c.re() == 5.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 5.0, true);

    c *= b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(32), true);
    BOOST_CHECK_EQUAL(c.re() == 7.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 7.0, true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(division_test, T, test_types) {
    typedef robust_fpt<T> rfpt_type;
    rfpt_type a(static_cast<T>(2), 3.0);
    rfpt_type b(static_cast<T>(4));
    rfpt_type c = a / b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(0.5), true);
    BOOST_CHECK_EQUAL(c.re() == 5.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 5.0, true);

    c /= b;
    BOOST_CHECK_EQUAL(c.fpv() == static_cast<T>(0.125), true);
    BOOST_CHECK_EQUAL(c.re() == 7.0, true);
    BOOST_CHECK_EQUAL(c.ulp() == 7.0, true);
}
