// Boost sweepline library segment_predicates_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "../test_type_list.hpp"
#include "boost/sweepline/voronoi_segment_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE predicates_test
#include <boost/test/test_case_template.hpp>

#define CHECK_ORIENTATION_EQUAL(p1, p2, p3, orientation) \
        BOOST_CHECK_EQUAL(detail::orientation_test(p1, p2, p3) == orientation, true)

#define CHECK_FAST_LESS_PREDICATE_PP(p1, p2, p3, exp) \
        BOOST_CHECK_EQUAL(detail::fast_less_predicate(p1, p2, p3) == exp, true)

#define CHECK_LESS_PREDICATE_PP(p1, p2, p3, exp) \
        BOOST_CHECK_EQUAL(detail::less_predicate(p1, p2, p3) == exp, true)

#define CHECK_LESS_PREDICATE_CHECK_PS(p1, p2, p3, p4, r, exp) \
        BOOST_CHECK_EQUAL(detail::less_predicate_check(p1, p2, p3, p4, r) == exp, true); \
        if (exp != detail::UNDEFINED) { \
            bool exp_res = (exp == detail::LESS) ? true : false; \
            BOOST_CHECK_EQUAL(detail::less_predicate(p1, p2, p3, p4, r), exp_res); \
        }

#define CHECK_LESS_PREDICATE_PS(p1, p2, p3, p4, r, exp) \
        BOOST_CHECK_EQUAL(detail::less_predicate(p1, p2, p3, p4, r) == exp, true)

#define CHECK_LESS_PREDICATE_SS(p1, p2, p3, p4, p5, r, exp) \
        BOOST_CHECK_EQUAL(detail::less_predicate(p1, p2, p3, p4, p5, r) == exp, true)

// This test uses integer values in the range [-2^31, 2^31), to validate
// orientation predicate for the hole integer range input coordinates.
BOOST_AUTO_TEST_CASE_TEMPLATE(orientation_test1, T, test_types) {
    int min_int = std::numeric_limits<int>::min();
    int max_int = std::numeric_limits<int>::max();
    point_2d<T> point1 = make_point_2d<T>(min_int, min_int);
    point_2d<T> point2 = make_point_2d<T>(0, 0);
    point_2d<T> point3 = make_point_2d<T>(max_int, max_int);
    point_2d<T> point4 = make_point_2d<T>(min_int, max_int);
    point_2d<T> point5 = make_point_2d<T>(max_int - 1, max_int);

    CHECK_ORIENTATION_EQUAL(point1, point2, point3, detail::COLINEAR);
    CHECK_ORIENTATION_EQUAL(point1, point3, point2, detail::COLINEAR);
    CHECK_ORIENTATION_EQUAL(point2, point3, point1, detail::COLINEAR);
    CHECK_ORIENTATION_EQUAL(point2, point1, point3, detail::COLINEAR);
    CHECK_ORIENTATION_EQUAL(point3, point1, point2, detail::COLINEAR);
    CHECK_ORIENTATION_EQUAL(point3, point2, point1, detail::COLINEAR);

    CHECK_ORIENTATION_EQUAL(point1, point4, point3, detail::RIGHT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point1, point3, point4, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point4, point1, point3, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point4, point3, point1, detail::RIGHT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point3, point4, point1, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point3, point1, point4, detail::RIGHT_ORIENTATION);

    CHECK_ORIENTATION_EQUAL(point1, point5, point3, detail::RIGHT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point1, point3, point5, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point5, point1, point3, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point5, point3, point1, detail::RIGHT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point3, point5, point1, detail::LEFT_ORIENTATION);
    CHECK_ORIENTATION_EQUAL(point3, point1, point5, detail::RIGHT_ORIENTATION);
}

// Test fast point-point predicate.
BOOST_AUTO_TEST_CASE_TEMPLATE(fast_less_predicate_test1, T, test_types) {
    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(0));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(-8), static_cast<T>(9));
    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(1));

    point_2d<T> site1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(5));
    CHECK_FAST_LESS_PREDICATE_PP(point1, point2, site1, detail::UNDEFINED);
    CHECK_FAST_LESS_PREDICATE_PP(point3, point1, site1, detail::UNDEFINED);

    point_2d<T> site2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(4));
    CHECK_FAST_LESS_PREDICATE_PP(point1, point2, site2, detail::MORE);
    CHECK_FAST_LESS_PREDICATE_PP(point3, point1, site2, detail::MORE);

    point_2d<T> site3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(6));
    CHECK_FAST_LESS_PREDICATE_PP(point1, point2, site3, detail::LESS);
    CHECK_FAST_LESS_PREDICATE_PP(point3, point1, site3, detail::LESS);
}

// Test main point-point predicate.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicates_point_point_test1, T, test_types) {
    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(0));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(-8), static_cast<T>(9));
    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(1));

    point_2d<T> site1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(5));
    CHECK_LESS_PREDICATE_PP(point1, point2, site1, false);
    CHECK_LESS_PREDICATE_PP(point3, point1, site1, false);

    point_2d<T> site2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(4));
    CHECK_LESS_PREDICATE_PP(point1, point2, site2, false);
    CHECK_LESS_PREDICATE_PP(point3, point1, site2, false);

    point_2d<T> site3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(6));
    CHECK_LESS_PREDICATE_PP(point1, point2, site3, true);
    CHECK_LESS_PREDICATE_PP(point3, point1, site3, true);
}

// Vertical segment case.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicate_check_point_segment_test1, T, test_types) {
    point_2d<T> segm_start = make_point_2d<T>(static_cast<T>(-4), static_cast<T>(0));
    point_2d<T> segm_end = make_point_2d<T>(static_cast<T>(-4), static_cast<T>(20));

    point_2d<T> site_p = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(10));
    point_2d<T> new_p1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(11));
    point_2d<T> new_p2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(9));

    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p, new_p1,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p, new_p2,
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p, new_p1,
                                  true, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p, new_p2,
                                  true, detail::UNDEFINED);
}

// Not vertical segment case. Site is to the left of the segment vector.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicate_check_point_segment_test2, T, test_types) {
    point_2d<T> segm_start = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(5));
    point_2d<T> segm_end = make_point_2d<T>(static_cast<T>(2), static_cast<T>(-2));
    
    point_2d<T> site_p1 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(4));
    point_2d<T> new_p1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-1));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p1, 
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p1, 
                                  true, detail::MORE);
    
    point_2d<T> new_p2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(1));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p2,
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p2,
                                  true, detail::UNDEFINED);

    point_2d<T> new_p3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(4));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p3,
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p3,
                                  true, detail::UNDEFINED);

    point_2d<T> new_p4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(5));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p4,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p4,
                                  true, detail::LESS);
}

// Not vertical segment case. Site is to the right of the segment vector.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicate_check_point_segment_test3, T, test_types) {
    point_2d<T> segm_start = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(5));
    point_2d<T> segm_end = make_point_2d<T>(static_cast<T>(2), static_cast<T>(-2));
    
    point_2d<T> site_p1 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(-4));
    point_2d<T> site_p2 = make_point_2d<T>(static_cast<int>(-4), static_cast<int>(1));

    point_2d<T> new_p1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(1));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p1, 
                                  false, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p1, 
                                  true, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p1, 
                                  false, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p1, 
                                  true, detail::LESS);
    
    point_2d<T> new_p2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-2));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p2,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p2,
                                  true, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p2,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p2,
                                  true, detail::LESS);

    point_2d<T> new_p3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-8));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p3,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p3,
                                  true, detail::LESS);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p3,
                                  false, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p3,
                                  true, detail::LESS);

    point_2d<T> new_p4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-9));
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p4,
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p1, new_p4,
                                  true, detail::UNDEFINED);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p4,
                                  false, detail::MORE);
    CHECK_LESS_PREDICATE_CHECK_PS(segm_start, segm_end, site_p2, new_p4,
                                  true, detail::UNDEFINED);
}

// Test main point-segment predicate.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicate_point_segment_test1, T, test_types) {
    point_2d<T> segm_start = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(5));
    point_2d<T> segm_end = make_point_2d<T>(static_cast<T>(2), static_cast<T>(-2));
    
    point_2d<T> site_p1 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(4));
    point_2d<T> site_p2 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(-4));
    point_2d<T> site_p3 = make_point_2d<T>(static_cast<int>(-4), static_cast<int>(1));

    point_2d<T> new_p1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(1));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p1, new_p1, true, false);

    point_2d<T> new_p2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(4));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p1, new_p2, true, true);

    point_2d<T> new_p3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(5));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p1, new_p3, false, false);

    point_2d<T> new_p4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(7));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p1, new_p4, false, true);

    point_2d<T> new_p5 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-2));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p2, new_p5, false, false);
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p3, new_p5, false, false);

    point_2d<T> new_p6 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-8));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p2, new_p6, false, false);
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p3, new_p6, false, false);

    point_2d<T> new_p7 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-9));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p2, new_p7, true, true);
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p3, new_p7, true, true);

    point_2d<T> new_p8 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-18));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p2, new_p8, true, false);
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p3, new_p8, true, false);

    point_2d<T> new_p9 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(-1));
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p2, new_p9, false, true);
    CHECK_LESS_PREDICATE_PS(segm_start, segm_end, site_p3, new_p9, false, true);
}

// Test main segment-segment predicate.
BOOST_AUTO_TEST_CASE_TEMPLATE(less_predicate_segment_segment_test1, T, test_types) {
    point_2d<T> segm_start1 = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(5));
    point_2d<T> segm_end1 = make_point_2d<T>(static_cast<T>(2), static_cast<T>(-2));

    // Common start point case.
    point_2d<T> segm_start2_1 = make_point_2d<T>(static_cast<T>(-5), static_cast<T>(5));
    point_2d<T> segm_end2_1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(6));
    point_2d<T> new_site1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(2));
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_1, segm_end2_1, 
                            new_site1, false, false);

    point_2d<T> new_site2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(9));
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_1, segm_end2_1, 
                            new_site2, false, true);

    point_2d<T> new_site3 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(10));
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_1, segm_end2_1, 
                            new_site3, false, true);

    // No common segment end points.
    point_2d<T> segm_start2_2 = make_point_2d<T>(static_cast<T>(-2), static_cast<T>(4));
    point_2d<T> segm_end2_2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(4));
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site1, false, false);
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site1, true, false);
    CHECK_LESS_PREDICATE_SS(segm_start2_2, segm_end2_2, segm_start1, segm_end1, 
                            new_site1, false, false);

    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site2, false, true);
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site2, true, true);
    CHECK_LESS_PREDICATE_SS(segm_start2_2, segm_end2_2, segm_start1, segm_end1, 
                            new_site2, false, true);

    point_2d<T> new_site4 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(5));
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site4, false, true);
    CHECK_LESS_PREDICATE_SS(segm_start1, segm_end1, segm_start2_2, segm_end2_2, 
                            new_site4, true, false);
    CHECK_LESS_PREDICATE_SS(segm_start2_2, segm_end2_2, segm_start1, segm_end1, 
                            new_site4, false, false);
}