// Boost sweepline library event_types_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_output.hpp"
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
    point_2d<T> point1 = make_point_2d(static_cast<T>(1), static_cast<T>(1.05));
    point_2d<T> point2;

    BOOST_CHECK_EQUAL(point1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(point1.y(), static_cast<T>(1.05));

    point2 = make_point_2d(static_cast<T>(0.999999), static_cast<T>(1));
    bool arr1[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr1);

    point2 = make_point_2d(static_cast<T>(1), static_cast<T>(1.1));
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr2);

    point2 = make_point_2d(static_cast<T>(1), static_cast<T>(1.05));
    bool arr3[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(point1, point2, arr3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(site_event_test1, T, test_types) {
    typedef point_2d<T> Point2D;

    site_event<Point2D> site1 = make_site_event<Point2D>(static_cast<T>(1),
                                                         static_cast<T>(1.05),
                                                         0);
    site_event<Point2D> site2;

    BOOST_CHECK_EQUAL(site1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(site1.y(), static_cast<T>(1.05));
    BOOST_CHECK_EQUAL(site1.get_site_index(), 0);

    site2 = make_site_event<Point2D>(static_cast<T>(0.999999), static_cast<T>(1), 1);
    bool arr1[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr1);

    site2 = make_site_event<Point2D>(static_cast<T>(1), static_cast<T>(1.1), 1);
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr2);

    site2 = make_site_event<Point2D>(static_cast<T>(1), static_cast<T>(1.05), 1);
    bool arr3[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(site1, site2, arr3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_test1, T, test_types) {
    typedef point_2d<T> Point2D;

    circle_event<Point2D> circle1 = make_circle_event<Point2D>(static_cast<T>(1),
                                                               static_cast<T>(2),
                                                               static_cast<T>(4));
    site_event<Point2D> temp_site = make_site_event<Point2D>(static_cast<T>(0),
                                                             static_cast<T>(0),
                                                             0);
    circle1.set_sites(temp_site, temp_site, temp_site);
    circle_event<Point2D> circle2;
    
    BOOST_CHECK_EQUAL(circle1.x(), static_cast<T>(1));
    BOOST_CHECK_EQUAL(circle1.y(), static_cast<T>(2));
    BOOST_CHECK_EQUAL(circle1.get_sqr_radius(), static_cast<T>(4));

    circle2 = make_circle_event<Point2D>(static_cast<T>(1),
                                         static_cast<T>(2),
                                         static_cast<T>(4));
    circle2.set_sites(temp_site, temp_site, temp_site);
    bool arr1[] = { false, false, true, true, true, false };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr1);

    circle2 = make_circle_event<Point2D>(static_cast<T>(1),
                                         static_cast<T>(3),
                                         static_cast<T>(4));
    circle2.set_sites(temp_site, temp_site, temp_site);
    bool arr2[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr2);

    circle2 = make_circle_event<Point2D>(static_cast<T>(1),
                                         static_cast<T>(2),
                                         static_cast<T>(5));
    circle2.set_sites(temp_site, temp_site, temp_site);
    bool arr3[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr3);


    circle2 = make_circle_event<Point2D>(static_cast<T>(0),
                                         static_cast<T>(2),
                                         static_cast<T>(4));
    circle2.set_sites(temp_site, temp_site, temp_site);
    bool arr4[] = { false, true, false, true, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr4);

    circle2 = make_circle_event<Point2D>(static_cast<T>(0),
                                         static_cast<T>(0),
                                         static_cast<T>(10));
    circle2.set_sites(temp_site, temp_site, temp_site);
    bool arr5[] = { true, false, true, false, false, true };
    EVENT_TYPES_CHECK_COMPARISON(circle1, circle2, arr5);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_test2, T, test_types) {
    typedef point_2d<T> Point2D;
    circle_event<Point2D> circle = make_circle_event<Point2D>(static_cast<T>(1),
                                                              static_cast<T>(2),
                                                              static_cast<T>(4));
    site_event<Point2D> site;

    site = make_site_event<Point2D>(0, 100, 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 1);

    site = make_site_event<Point2D>(3, 0, 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 1);

    site = make_site_event<Point2D>(3, 2, 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 0);
    
    site = make_site_event<Point2D>(3, 2, 0);
    BOOST_CHECK_EQUAL(circle.compare(site), 0);

    site = make_site_event<Point2D>(4, 2, 0);
    BOOST_CHECK_EQUAL(circle.compare(site), -1);
}