// Boost sweepline library node_comparer_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include "boost/sweepline/detail/voronoi_formation.hpp"
using namespace boost::sweepline::detail;

#define BOOST_TEST_MODULE node_comparer_test
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test1, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    typedef std::map< bline_node, int, node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0),
        make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1));
    test_beach_line[initial_node] = 0;
    BOOST_CHECK_EQUAL(test_beach_line.size(), 1);
    
    bline_node new_node1(make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 3));
    bline_node new_node3(make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 4));
    bline_node new_node4(make_site_event<T>(static_cast<T>(1), static_cast<T>(1.000001), 5));
    bline_node new_node5(make_site_event<T>(static_cast<T>(1), static_cast<T>(0.999999), 6));
    
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

    it = test_beach_line.lower_bound(initial_node);
    BOOST_CHECK_EQUAL(it == test_beach_line.begin(), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test2, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    typedef std::map< bline_node, int, node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    site_event<T> site1 = make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0);
    site_event<T> site2 = make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1);
    bline_node initial_node(site1, site2);
    test_beach_line[initial_node] = 2;

    site_event<T> site3 = make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 2);
    bline_node node1(site1, site3);
    bline_node node2(site3, site1);
    test_beach_line.insert(std::pair< bline_node, int>(node1, 0));
    test_beach_line.insert(std::pair< bline_node, int>(node2, 1));
    
    int cur_value = 0;
    for (bline_it it = test_beach_line.begin();
         it != test_beach_line.end();
         it++, cur_value++)
        BOOST_CHECK_EQUAL(it->second, cur_value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test3, T, test_types) {
    typedef beach_line_node< point_2d<T> > bline_node;
    typedef std::map< bline_node, int, node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    site_event<T> site1 = make_site_event<T>(static_cast<T>(0), static_cast<T>(1), 0);
    site_event<T> site2 = make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 1);
    site_event<T> site3 = make_site_event<T>(static_cast<T>(2), static_cast<T>(4), 2);
    bline_node initial_node1(site1, site2);
    bline_node initial_node2(site2, site1);
    test_beach_line[initial_node1] = 0;
    test_beach_line[initial_node2] = 1;

    bline_node new_node1(site1, site3);
    bline_node new_node2(site3, site1);
    test_beach_line[new_node1] = 2;
    test_beach_line[new_node2] = 3;

    int cur_value = 0;
    for (bline_it it = test_beach_line.begin();
         it != test_beach_line.end();
         it++, cur_value++)
        BOOST_CHECK_EQUAL(it->second, cur_value);
}