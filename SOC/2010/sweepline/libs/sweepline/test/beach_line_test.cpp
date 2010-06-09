// Boost sweepline library beach_line_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include <boost/sweepline/beach_line.hpp>
#include <boost/sweepline/event_types.hpp>
using namespace boost::sweepline;

#define BOOST_TEST_MODULE beach_line_test
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_vertex_computation_test1, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    bline_node initial_node(make_point_2d<T>(static_cast<T>(0), static_cast<T>(0)),
                            make_point_2d<T>(static_cast<T>(0), static_cast<T>(2)));

    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(1), static_cast<T>(0));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(1), static_cast<T>(1));
    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(1), static_cast<T>(2));

    point_2d<T> voronoi_vertex1 = initial_node.get_intersection_point(false, point1);
    BOOST_CHECK_EQUAL(voronoi_vertex1.x() == static_cast<T>(0.5) &&
                      voronoi_vertex1.y() == static_cast<T>(0.0), true);

    point_2d<T> voronoi_vertex2_1 = initial_node.get_intersection_point(false, point2);
    point_2d<T> voronoi_vertex2_2 = initial_node.get_intersection_point(true, point2);
    BOOST_CHECK_EQUAL(voronoi_vertex2_1.x() == static_cast<T>(0.0) &&
                      voronoi_vertex2_1.y() == static_cast<T>(1.0) &&
                      voronoi_vertex2_1.x() == voronoi_vertex2_2.x() &&
                      voronoi_vertex2_1.y() == voronoi_vertex2_2.y(), true);

    point_2d<T> voronoi_vertex3 = initial_node.get_intersection_point(true, point3);
    BOOST_CHECK_EQUAL(voronoi_vertex3.x() == static_cast<T>(0.5) &&
                      voronoi_vertex3.y() == static_cast<T>(2.0), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test1, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    typedef std::map< bline_node, int, node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    bline_node initial_node(make_point_2d<T>(static_cast<T>(0), static_cast<T>(0)),
                            make_point_2d<T>(static_cast<T>(0), static_cast<T>(2)));
    test_beach_line[initial_node] = 0;
    BOOST_CHECK_EQUAL(test_beach_line.size(), 1);
    
    bline_node new_node1(make_point_2d<T>(static_cast<T>(1), static_cast<T>(0)));
    bline_node new_node2(make_point_2d<T>(static_cast<T>(1), static_cast<T>(1)));
    bline_node new_node3(make_point_2d<T>(static_cast<T>(1), static_cast<T>(2)));
    bline_node new_node4(make_point_2d<T>(static_cast<T>(1), static_cast<T>(1.000001)));
    bline_node new_node5(make_point_2d<T>(static_cast<T>(1), static_cast<T>(0.999999)));
    
    bline_it it = test_beach_line.lower_bound(new_node1);
    BOOST_CHECK_EQUAL(it == test_beach_line.begin(), true);

    it = test_beach_line.lower_bound(new_node2);
    BOOST_CHECK_EQUAL(it == test_beach_line.begin(), true);

    it = test_beach_line.lower_bound(new_node3);
    BOOST_CHECK_EQUAL(it == test_beach_line.end(), true);

    it = test_beach_line.lower_bound(new_node4);
    BOOST_CHECK_EQUAL(it == test_beach_line.end(), true);

    it = test_beach_line.lower_bound(new_node5);
    BOOST_CHECK_EQUAL(it == test_beach_line.begin(), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test2, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    typedef std::map< bline_node, int, node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    point_2d<T> point1 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(0));
    point_2d<T> point2 = make_point_2d<T>(static_cast<T>(0), static_cast<T>(2));
    bline_node initial_node(point1, point2);
    test_beach_line[initial_node] = 0;

    point_2d<T> point3 = make_point_2d<T>(static_cast<T>(1), static_cast<T>(0));
    bline_node node1(point1, point3);
    bline_node node2(point3, point1);
    test_beach_line.insert(std::pair< bline_node, int>(node1, 1));
    test_beach_line.insert(std::pair< bline_node, int>(node2, 2));
    
    bline_it it = test_beach_line.begin();
    BOOST_CHECK_EQUAL(it->second == 1, true);

    ++it;
    BOOST_CHECK_EQUAL(it->second == 2, true);

    ++it;
    BOOST_CHECK_EQUAL(it->second == 0, true);
}