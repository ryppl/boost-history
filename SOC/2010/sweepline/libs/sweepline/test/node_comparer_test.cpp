// Boost sweepline library node_comparer_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;
using namespace boost::sweepline::detail;

#define BOOST_TEST_MODULE node_comparer_test
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp1, T, test_types) {
    typedef site_event<T> site_event_type;
    typedef beach_line_node<T> bline_node;
    typedef typename std::map< bline_node, int,
        node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    site_event_type site1 = make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0);
    site_event_type site2 = make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1);
    bline_node initial_node(site1, site2);
    test_beach_line[initial_node] = 2;

    site_event_type site3 = make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 2);
    bline_node node1(site1, site3);
    bline_node node2(site3, site1);
    test_beach_line.insert(std::pair<bline_node, int>(node1, 0));
    test_beach_line.insert(std::pair<bline_node, int>(node2, 1));

    int cur_value = 0;
    for (bline_it it = test_beach_line.begin();
         it != test_beach_line.end();
         it++, cur_value++)
        BOOST_CHECK_EQUAL(it->second, cur_value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp2, T, test_types) {
    typedef site_event<T> site_event_type;
    typedef beach_line_node<T> bline_node;
    typedef typename std::map< bline_node, int,
        node_comparer<bline_node> >::const_iterator bline_it;

    std::map< bline_node, int, node_comparer<bline_node> > test_beach_line;

    site_event_type site1 = make_site_event<T>(static_cast<T>(0), static_cast<T>(1), 0);
    site_event_type site2 = make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 1);
    site_event_type site3 = make_site_event<T>(static_cast<T>(2), static_cast<T>(4), 2);
    bline_node initial_node1(site1, site2);
    bline_node initial_node2(site2, site1);
    test_beach_line.insert(std::pair<bline_node, int>(initial_node1, 0));
    test_beach_line.insert(std::pair<bline_node, int>(initial_node2, 1));

    bline_node new_node1(site1, site3);
    bline_node new_node2(site3, site1);
    test_beach_line.insert(std::pair<bline_node, int>(new_node1, 2));
    test_beach_line.insert(std::pair<bline_node, int>(new_node2, 3));

    int cur_value = 0;
    for (bline_it it = test_beach_line.begin();
         it != test_beach_line.end();
         it++, cur_value++)
        BOOST_CHECK_EQUAL(it->second, cur_value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp3, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 0),
        make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(2), static_cast<T>(-10), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(2), static_cast<T>(-1), 3));
    bline_node new_node3(make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 4));
    bline_node new_node4(make_site_event<T>(static_cast<T>(2), static_cast<T>(1), 4));
    bline_node new_node5(make_site_event<T>(static_cast<T>(2), static_cast<T>(2), 4));
    bline_node new_node6(make_site_event<T>(static_cast<T>(2), static_cast<T>(3), 4));


    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node4), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node5), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node6), true);


    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node4, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node5, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node6, initial_node), false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp4, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(0), static_cast<T>(1), 0),
        make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(2), static_cast<T>(-3), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(2), static_cast<T>(-2), 2));
    bline_node new_node3(make_site_event<T>(static_cast<T>(2), static_cast<T>(-1), 2));
    bline_node new_node4(make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 2));
    bline_node new_node5(make_site_event<T>(static_cast<T>(2), static_cast<T>(1), 2));
    bline_node new_node6(make_site_event<T>(static_cast<T>(2), static_cast<T>(3), 2));

    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node4), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node5), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node6), true);

    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node4, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node5, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node6, initial_node), false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp5, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0),
        make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(2), static_cast<T>(-10), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 3));
    bline_node new_node3(make_site_event<T>(static_cast<T>(2), static_cast<T>(1), 4));
    bline_node new_node4(make_site_event<T>(static_cast<T>(2), static_cast<T>(2), 4));
    bline_node new_node5(make_site_event<T>(static_cast<T>(2), static_cast<T>(5), 4));
    bline_node new_node6(make_site_event<T>(static_cast<T>(2), static_cast<T>(20), 4));


    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node4), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node5), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node6), true);


    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node4, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node5, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node6, initial_node), false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp6, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 0),
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(2), static_cast<T>(-3), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(2), static_cast<T>(-2), 3));
    bline_node new_node3(make_site_event<T>(static_cast<T>(2), static_cast<T>(0), 4));
    bline_node new_node4(make_site_event<T>(static_cast<T>(2), static_cast<T>(1), 4));
    bline_node new_node5(make_site_event<T>(static_cast<T>(2), static_cast<T>(2), 4));
    bline_node new_node6(make_site_event<T>(static_cast<T>(2), static_cast<T>(3), 4));
    bline_node new_node7(make_site_event<T>(static_cast<T>(2), static_cast<T>(5), 4));

    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node4), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node5), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node6), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node7), true);

    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node4, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node5, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node6, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node7, initial_node), false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp7, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0),
        make_site_event<T>(static_cast<T>(0), static_cast<T>(2), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 3));
    bline_node new_node3(make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 4));

    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), true);

    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), false);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(node_comparer_test_pp8, T, test_types) {
    typedef point_2d<T> Point2D;
    typedef beach_line_node<T> bline_node;
    node_comparer<bline_node> node_comparer_test;

    bline_node initial_node(
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0),
        make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 1));

    bline_node new_node1(make_site_event<T>(static_cast<T>(1), static_cast<T>(0), 2));
    bline_node new_node2(make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 1));
    bline_node new_node3(make_site_event<T>(static_cast<T>(1), static_cast<T>(2), 3));
    bline_node new_node4(
        make_site_event<T>(static_cast<T>(1), static_cast<T>(1), 1),
        make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0));

    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node1), false);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node2), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node3), true);
    BOOST_CHECK_EQUAL(node_comparer_test(initial_node, new_node4), true);

    BOOST_CHECK_EQUAL(node_comparer_test(new_node1, initial_node), true);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node2, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node3, initial_node), false);
    BOOST_CHECK_EQUAL(node_comparer_test(new_node4, initial_node), false);
}
