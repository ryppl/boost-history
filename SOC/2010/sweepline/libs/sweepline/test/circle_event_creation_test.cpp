// Boost sweepline library circle_event_creation_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE circle_event_creation
#include <boost/test/test_case_template.hpp>

#define CHECK_CIRCLE_EVENT(c_e, c_x, c_y, l_x) \
    BOOST_CHECK_EQUAL(detail::almost_equal(c_e.x(), static_cast<T>(c_x), 10), true); \
    BOOST_CHECK_EQUAL(detail::almost_equal(c_e.y(), static_cast<T>(c_y), 10), true); \
    BOOST_CHECK_EQUAL(detail::almost_equal(c_e.lower_x(), static_cast<T>(l_x), 10), true)

// Test for (point, point, point) case.
BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_ppp_test1, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    site_event_type site1(static_cast<T>(0), static_cast<T>(0), 0);
    site_event_type site2(static_cast<T>(-8), static_cast<T>(0), 1);
    site_event_type site3(static_cast<T>(0), static_cast<T>(6), 2);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_ppp(site1, site2, site3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, -4, 3, 1);
    is_event = detail::create_circle_event_ppp(site2, site1, site3, c_event);
    BOOST_CHECK_EQUAL(is_event, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_ppp_test2, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    int min_int = std::numeric_limits<int>::min();
    int max_int = std::numeric_limits<int>::max();
    site_event_type site1(static_cast<T>(min_int), static_cast<T>(min_int), 0);
    site_event_type site2_1(static_cast<T>(min_int), static_cast<T>(max_int), 1);
    site_event_type site2_2(static_cast<T>(max_int-1), static_cast<T>(max_int-1), 1);
    site_event_type site3(static_cast<T>(max_int), static_cast<T>(max_int), 2);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_ppp(site1, site2_1, site3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    is_event = detail::create_circle_event_ppp(site1, site2_2, site3, c_event);
    BOOST_CHECK_EQUAL(is_event, false);
}

// Test for (point, point, segment) case.
BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pps_test1, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    site_event_type segm_start(static_cast<T>(-4), static_cast<T>(0), 0);
    site_event_type segm_end(static_cast<T>(0), static_cast<T>(4), 0);
    site_event_type segm_site(segm_start.point0(), segm_end.point0(), 0);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_pps(segm_start, segm_end, segm_site, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, false);
    site_event_type site1_1(static_cast<T>(-2), static_cast<T>(0), 0);
    site_event_type site1_2(static_cast<T>(0), static_cast<T>(2), 0);
    is_event = detail::create_circle_event_pps(site1_1, site1_2, segm_site, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    BOOST_CHECK_EQUAL(c_event.center().x() == static_cast<T>(-1), true);
    BOOST_CHECK_EQUAL(c_event.center().y() == static_cast<T>(1), true);
    is_event = detail::create_circle_event_pps(site1_1, site1_2, segm_site, 1, c_event);
    BOOST_CHECK_EQUAL(is_event, false);
    is_event = detail::create_circle_event_pps(site1_1, site1_2, segm_site, 3, c_event);
    BOOST_CHECK_EQUAL(is_event, false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pps_test2, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    site_event_type segm_start(static_cast<T>(-4), static_cast<T>(0), 0);
    site_event_type segm_end(static_cast<T>(-4), static_cast<T>(20), 0);
    site_event_type segm_site(segm_start.point0(), segm_end.point0(), 0);
    typename detail::circle_event<T> c_event;
    site_event_type site1_1(static_cast<T>(-2), static_cast<T>(10), 0);
    site_event_type site1_2(static_cast<T>(4), static_cast<T>(10), 0);
    bool is_event = detail::create_circle_event_pps(site1_1, site1_2, segm_site, 1, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 6, 6);
    is_event = detail::create_circle_event_pps(site1_2, site1_1, segm_site, 3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 14, 6);
}

// Test for (point, segment, segment) case.
BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pss_test1, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> segm1_start(static_cast<T>(1), static_cast<T>(0));
    point_2d<T> segm1_end(static_cast<T>(7), static_cast<T>(0));
    site_event_type segm_site1(segm1_start, segm1_end, 0);
    segm_site1.inverse();
    point_2d<T> segm2_start(static_cast<T>(-2), static_cast<T>(4));
    point_2d<T> segm2_end(static_cast<T>(10), static_cast<T>(4));
    site_event_type segm_site2(segm2_start, segm2_end, 1);
    typename detail::circle_event<T> c_event;

    site_event_type site1(static_cast<T>(6), static_cast<T>(2), 2);
    bool is_event = detail::create_circle_event_pss(site1, segm_site1, segm_site2, 3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 4, 2, 6);

    site_event_type site2(static_cast<T>(1), static_cast<T>(0), 2);
    is_event = detail::create_circle_event_pss(site2, segm_site1, segm_site2, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 2, 3);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pss_test2, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> segm1_start(static_cast<T>(-1), static_cast<T>(2));
    point_2d<T> segm1_end(static_cast<T>(8), static_cast<T>(-10));
    site_event_type segm_site1(segm1_start, segm1_end, 0);
    segm_site1.inverse();
    point_2d<T> segm2_start(static_cast<T>(-1), static_cast<T>(0));
    point_2d<T> segm2_end(static_cast<T>(8), static_cast<T>(12));
    site_event_type segm_site2(segm2_start, segm2_end, 1);
    typename detail::circle_event<T> c_event;
    site_event_type site1(static_cast<T>(1), static_cast<T>(1), 2);
    bool is_event = detail::create_circle_event_pss(site1, segm_site1, segm_site2, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 6, 1, 11);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pss_test3, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> segm1_start(static_cast<T>(1), static_cast<T>(0));
    point_2d<T> segm1_end(static_cast<T>(6), static_cast<T>(0));
    site_event_type segm_site1(segm1_start, segm1_end, 0);
    segm_site1.inverse();
    point_2d<T> segm2_start(static_cast<T>(-6), static_cast<T>(4));
    point_2d<T> segm2_end(static_cast<T>(0), static_cast<T>(12));
    site_event_type segm_site2(segm2_start, segm2_end, 1);
    typename detail::circle_event<T> c_event;
    site_event_type site1(static_cast<T>(1), static_cast<T>(0), 2);
    bool is_event = detail::create_circle_event_pss(site1, segm_site1, segm_site2, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 5, 6);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_pss_test4, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> point1(static_cast<T>(1), static_cast<T>(0));
    point_2d<T> point2(static_cast<T>(5), static_cast<T>(0));
    point_2d<T> point3(static_cast<T>(8), static_cast<T>(6));
    point_2d<T> point4(static_cast<T>(0), static_cast<T>(12));
    site_event_type segm_site1(point1, point2, 0);
    segm_site1.inverse();
    site_event_type segm_site2(point3, point4, 1);
    site_event_type point_site(point1, 2);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_pss(
        point_site, segm_site1, segm_site2, 2, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 5, 6);
}

// Test for (segment, segment, segment) case.
BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_sss_test1, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> point1(static_cast<T>(4), static_cast<T>(0));
    point_2d<T> point2(static_cast<T>(0), static_cast<T>(0));
    point_2d<T> point3(static_cast<T>(0), static_cast<T>(4));
    point_2d<T> point4(static_cast<T>(4), static_cast<T>(4));
    site_event_type segm_site1(point1, point2, 0);
    segm_site1.inverse();
    site_event_type segm_site2(point2, point3, 1);
    site_event_type segm_site3(point3, point4, 2);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_sss(segm_site1, segm_site2, segm_site3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 2, 2, 4);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(circle_event_creation_sss_test2, T, test_types) {
    typedef typename detail::site_event<T> site_event_type;
    point_2d<T> point1(static_cast<T>(41), static_cast<T>(30));
    point_2d<T> point2(static_cast<T>(1), static_cast<T>(0));
    point_2d<T> point3(static_cast<T>(-39), static_cast<T>(30));
    point_2d<T> point4(static_cast<T>(1), static_cast<T>(60));
    site_event_type segm_site1(point1, point2, 0);
    segm_site1.inverse();
    site_event_type segm_site2(point3, point4, 1);
    site_event_type segm_site3(point4, point1, 2);
    typename detail::circle_event<T> c_event;
    bool is_event = detail::create_circle_event_sss(segm_site1, segm_site2, segm_site3, c_event);
    BOOST_CHECK_EQUAL(is_event, true);
    CHECK_CIRCLE_EVENT(c_event, 1, 30, 25);
}
