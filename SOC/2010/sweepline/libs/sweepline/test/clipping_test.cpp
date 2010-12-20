// Boost sweepline library clipping_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_clipping_test
#include <boost/test/test_case_template.hpp>

// Test segment clipping.
BOOST_AUTO_TEST_CASE_TEMPLATE(segment_clipping_test1, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(2.0));
    point_2d<T> test_origin(static_cast<T>(-1), static_cast<T>(3));
    point_2d<T> test_direction1_1(static_cast<T>(8), static_cast<T>(-8));
    point_2d<T> test_direction1_2(static_cast<T>(2), static_cast<T>(-2));
    point_2d<T> test_direction1_3(static_cast<T>(0.5), static_cast<T>(-0.5));
    point_2d<T> test_direction2(static_cast<T>(2), static_cast<T>(-4));
    point_2d<T> test_direction3(static_cast<T>(2), static_cast<T>(-1));
    point_2d<T> test_direction4(static_cast<T>(1), static_cast<T>(-4));
    point_2d<T> test_direction5(static_cast<T>(5), static_cast<T>(-1));

    std::vector< point_2d<T> > intersections;
    voronoi_helper<T>::find_intersections(test_origin, test_direction1_1,
                                          voronoi_helper<T>::SEGMENT,
                                          test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(2), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(3) &&
                      intersections[1].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction1_2,
                                  voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(2), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction1_3,
                                  voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(intersections.empty(), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction2,
                                   voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(1), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(1) &&
                      intersections[1].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction3,
                                   voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(1) &&
                      intersections[0].y() == static_cast<T>(2), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction4,
                                   voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction5,
                                   voronoi_helper<T>::SEGMENT, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(4) &&
                      intersections[0].y() == static_cast<T>(2), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_clipping_test2, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(3.0));
    std::vector< point_2d<T> > intersections;
    srand(static_cast<unsigned int>(time(NULL)));
    point_2d<T> test_origin(2, 1);

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            intersections.clear();
            point_2d<T> test_direction(static_cast<T>(i), static_cast<T>(j));
            voronoi_helper<T>::find_intersections(test_origin, test_direction,
                voronoi_helper<T>::SEGMENT, test_rect, intersections);
            if (abs(i) >= 2 || abs(j) >= 2)
                BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
            else
                BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 0);
        }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_clipping_test3, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(3.0));
    std::vector< point_2d<T> > intersections;
    srand(static_cast<unsigned int>(time(NULL)));
    point_2d<T> test_origin(2, 1);

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            intersections.clear();
            T x = static_cast<T>(i) / static_cast<T>(26);
            T y = static_cast<T>(j) / static_cast<T>(26);
            point_2d<T> test_direction(x, y);
            voronoi_helper<T>::find_intersections(test_origin, test_direction,
                voronoi_helper<T>::SEGMENT, test_rect, intersections);
            BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 0);
        }
}

// Test ray clipping.
BOOST_AUTO_TEST_CASE_TEMPLATE(ray_clipping_test1, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(2.0));
    point_2d<T> test_origin(static_cast<T>(-1), static_cast<T>(3));
    point_2d<T> test_direction1(static_cast<T>(2), static_cast<T>(-2));
    point_2d<T> test_direction2(static_cast<T>(2), static_cast<T>(-4));
    point_2d<T> test_direction3(static_cast<T>(2), static_cast<T>(-1));
    point_2d<T> test_direction4(static_cast<T>(1), static_cast<T>(-4));
    point_2d<T> test_direction5(static_cast<T>(5), static_cast<T>(-1));

    std::vector< point_2d<T> > intersections;
    voronoi_helper<T>::find_intersections(test_origin, test_direction1,
        voronoi_helper<T>::RAY, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(2), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(3) &&
                      intersections[1].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction2,
        voronoi_helper<T>::RAY, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(1), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(1) &&
                      intersections[1].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction3,
        voronoi_helper<T>::RAY, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(1) &&
                      intersections[0].y() == static_cast<T>(2), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(4) &&
                      intersections[1].y() == static_cast<T>(0.5), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction4,
        voronoi_helper<T>::RAY, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction5,
        voronoi_helper<T>::RAY, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(4) &&
                      intersections[0].y() == static_cast<T>(2), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ray_clipping_test2, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(3.0));
    std::vector< point_2d<T> > intersections;
    srand(static_cast<unsigned int>(time(NULL)));
    point_2d<T> test_origin(2, 1);

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            intersections.clear();
            T x = static_cast<T>(i) / static_cast<T>(26);
            T y = static_cast<T>(j) / static_cast<T>(26);
            point_2d<T> test_direction(x, y);
            voronoi_helper<T>::find_intersections(test_origin, test_direction,
                voronoi_helper<T>::RAY, test_rect, intersections);
            if (i && j)
                BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
        }
}

// Test line clipping.
BOOST_AUTO_TEST_CASE_TEMPLATE(line_clipping_test1, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(2.0));
    point_2d<T> test_origin(static_cast<T>(-1), static_cast<T>(3));
    point_2d<T> test_direction1(static_cast<T>(-1), static_cast<T>(1));
    point_2d<T> test_direction2(static_cast<T>(-1), static_cast<T>(2));
    point_2d<T> test_direction3(static_cast<T>(-2), static_cast<T>(1));
    point_2d<T> test_direction4(static_cast<T>(-1), static_cast<T>(4));
    point_2d<T> test_direction5(static_cast<T>(-5), static_cast<T>(1));

    std::vector< point_2d<T> > intersections;
    voronoi_helper<T>::find_intersections(test_origin, test_direction1,
        voronoi_helper<T>::LINE, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(3) &&
                      intersections[0].y() == static_cast<T>(-1), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(0) &&
                      intersections[1].y() == static_cast<T>(2), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction2,
        voronoi_helper<T>::LINE, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(1) &&
                      intersections[0].y() == static_cast<T>(-1), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(0) &&
                      intersections[1].y() == static_cast<T>(1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction3,
        voronoi_helper<T>::LINE, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(4) &&
                      intersections[0].y() == static_cast<T>(0.5), true);
    BOOST_CHECK_EQUAL(intersections[1].x() == static_cast<T>(1) &&
                      intersections[1].y() == static_cast<T>(2), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction4,
        voronoi_helper<T>::LINE, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(0) &&
                      intersections[0].y() == static_cast<T>(-1), true);

    intersections.clear();
    voronoi_helper<T>::find_intersections(test_origin, test_direction5,
        voronoi_helper<T>::LINE, test_rect, intersections);
    BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 1);
    BOOST_CHECK_EQUAL(intersections[0].x() == static_cast<T>(4) &&
                      intersections[0].y() == static_cast<T>(2), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(line_clipping_test2, T, test_types) {
    BRect<T> test_rect(static_cast<T>(0.0), static_cast<T>(-1.0),
                       static_cast<T>(4.0), static_cast<T>(3.0));
    std::vector< point_2d<T> > intersections;
    srand(static_cast<unsigned int>(time(NULL)));
    point_2d<T> test_origin(2, 1);

    for (int i = -50; i <= 50; i++)
        for (int j = -50; j <= 50; j++) {
            intersections.clear();
            T x = static_cast<T>(i) / static_cast<T>(26);
            T y = static_cast<T>(j) / static_cast<T>(26);
            point_2d<T> test_direction(x, y);
            voronoi_helper<T>::find_intersections(test_origin, test_direction,
                voronoi_helper<T>::LINE, test_rect, intersections);
            if (i && j)
                BOOST_CHECK_EQUAL(static_cast<int>(intersections.size()), 2);
        }
}
