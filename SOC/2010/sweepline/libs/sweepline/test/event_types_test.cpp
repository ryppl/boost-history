// Boost sweepline library event_types_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;
using namespace boost::sweepline::detail;

#define BOOST_TEST_MODULE event_types_test
#include <boost/test/test_case_template.hpp>

#define EVENT_TYPES_CHECK_COMPARISON(A, B, ARR)    \
            BOOST_CHECK_EQUAL((A)<(B), (ARR)[0]);  \
            BOOST_CHECK_EQUAL((A)>(B), (ARR)[1]);  \
            BOOST_CHECK_EQUAL((A)<=(B), (ARR)[2]); \
            BOOST_CHECK_EQUAL((A)>=(B), (ARR)[3]); \
            BOOST_CHECK_EQUAL((A)==(B), (ARR)[4]); \
            BOOST_CHECK_EQUAL((A)!=(B), (ARR)[5])

BOOST_AUTO_TEST_CASE_TEMPLATE(point_2d_test1, T, test_types) {
    point_2d<T> point1 = make_point_2d(static_cast<T>(1), static_cast<T>(2));
    point_2d<T> point2;

    BOOST_CHECK_EQUAL(point1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(point1.y(), static_cast<T>(2));

    point2 = make_point_2d(static_cast<T>(0), static_cast<T>(2));
    bool arr1[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr1);

    point2 = make_point_2d(static_cast<T>(1), static_cast<T>(3));
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr2);

    point2 = make_point_2d(static_cast<T>(1), static_cast<T>(2));
    bool arr3[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(site_event_test1, T, test_types) {
    site_event<T> site1 = make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 0);
    site_event<T> site2;

    BOOST_CHECK_EQUAL(site1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(site1.y(), static_cast<T>(2));
    BOOST_CHECK_EQUAL(site1.index(), 0);

    site2 = make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1);
    bool arr1[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1);

    site2 = make_site_event<T>(static_cast<T>(1), static_cast<T>(3), 1);
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr2);

    site2 = make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 1);
    bool arr3[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr3);
}


BOOST_AUTO_TEST_CASE_TEMPLATE(site_event_test2, T, test_types) {
    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(2));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(0));
    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-2));
    point_2d<T> point4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-1));
    point_2d<T> point5 = make_point_2d<T>(static_cast<T>(1), static_cast<T>(1));
    site_event<T> site1 = make_site_event<T>(point1, point2, 0);

    BOOST_CHECK_EQUAL(point1 == site1.point1(), true);
    BOOST_CHECK_EQUAL(point2 == site1.point0(), true);
    BOOST_CHECK_EQUAL(site1.is_segment(), true);
    BOOST_CHECK_EQUAL(site1.is_vertical(), true);

    site_event<T> site2 = make_site_event<T>(point1, 0);
    BOOST_CHECK_EQUAL(site2.is_segment(), false);
    BOOST_CHECK_EQUAL(site2.is_vertical(), true);

    bool arr1_1[] = { true, false, true, false, false, true };
    bool arr1_2[] = { false , true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    site2 = make_site_event<T>(point2, 0);
    bool arr2_1[] = { false, true, false, true, false, true };
    bool arr2_2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr2_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr2_2);

    site2 = make_site_event<T>(point3, point4, 0);
    bool arr3_1[] = { false, true, false, true, false, true };
    bool arr3_2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr3_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr3_2);

    site2 = make_site_event<T>(point3, point5, 0);
    bool arr4_1[] = { true, false, true, false, false, true };
    bool arr4_2[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr4_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr4_2);

    site2 = make_site_event<T>(point2, point5, 0);
    bool arr5_1[] = { true, false, true, false, false, true };
    bool arr5_2[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr5_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr5_2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(site_event_test3, T, test_types) {
    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(10), static_cast<T>(10));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(0));
    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(1));
    point_2d<T> point4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(10));
    site_event<T> site1 = make_site_event<T>(point1, point2, 0);

    BOOST_CHECK_EQUAL(point1 == site1.point1(), true);
    BOOST_CHECK_EQUAL(point2 == site1.point0(), true);
    BOOST_CHECK_EQUAL(site1.is_segment(), true);
    BOOST_CHECK_EQUAL(site1.is_vertical(), false);

    site_event<T> site2 = make_site_event<T>(point2, 0);
    bool arr1_1[] = { false, true, false, true, false, true };
    bool arr1_2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    site2 = make_site_event<T>(static_cast<T>(0), static_cast<T>(-1), 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    site2 = make_site_event<T>(static_cast<T>(0), static_cast<T>(1), 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    site2 = make_site_event<T>(point3, point4, 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    point3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-10));
    point4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-1));
    site2 = make_site_event<T>(point3, point4, 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);

    point4 = make_point_2d<T>(static_cast<T>(10), static_cast<T>(9));
    site2 = make_site_event<T>(point2, point4, 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_2);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_1);

    point4 = make_point_2d<T>(static_cast<T>(9), static_cast<T>(10));
    site2 = make_site_event<T>(point2, point4, 0);
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1_1);
    EVENT_TYPES_CHECK_COMPARISON(site2, site1, arr1_2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_test1, T, test_types) {
    circle_event<T> circle1 = make_circle_event<T>(static_cast<T>(1),
                                                   static_cast<T>(2),
                                                   static_cast<T>(3));
    site_event<T> temp_site = make_site_event<T>(static_cast<T>(0), static_cast<T>(0),0);
    circle_event<T> circle2;

    BOOST_CHECK_EQUAL(circle1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(circle1.y(), static_cast<T>(2));
    BOOST_CHECK_EQUAL(circle1.lower_x(), static_cast<T>(3));

    circle2 = make_circle_event<T>(static_cast<T>(1), static_cast<T>(2), static_cast<T>(3));
    bool arr1[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr1);

    circle2 = make_circle_event<T>(static_cast<T>(1), static_cast<T>(3), static_cast<T>(3));
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr2);

    circle2 = make_circle_event<T>(static_cast<T>(1), static_cast<T>(2), static_cast<T>(4));
    bool arr3[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr3);

    circle2 = make_circle_event<T>(static_cast<T>(0), static_cast<T>(2), static_cast<T>(2));
    bool arr4[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr4);

    circle2 = make_circle_event<T>(static_cast<T>(-1), static_cast<T>(2), static_cast<T>(3));
    bool arr5[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr5);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_test2, T, test_types) {
    circle_event<T> circle = make_circle_event<T>(static_cast<T>(1),
                                                  static_cast<T>(2),
                                                  static_cast<T>(3));
    site_event<T> site;

    site = make_site_event<T>(static_cast<T>(0), static_cast<T>(100), 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 1);

    site = make_site_event<T>(static_cast<T>(3), static_cast<T>(0), 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 1);

    site = make_site_event<T>(static_cast<T>(3), static_cast<T>(2), 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 0);

    site = make_site_event<T>(static_cast<T>(3), static_cast<T>(3), 0);
    BOOST_CHECK_EQUAL(circle.compare(site), -1);

    site = make_site_event<T>(static_cast<T>(4), static_cast<T>(2), 0);
    BOOST_CHECK_EQUAL(circle.compare(site), -1);
}
