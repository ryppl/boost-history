// Boost sweepline library event_queue_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include <boost/sweepline/event_queue.hpp>
#include <boost/sweepline/event_types.hpp>
using namespace boost::sweepline;

#define BOOST_TEST_MODULE event_queue_test
#include <boost/test/test_case_template.hpp>

#define CHECK_TOP_ELEMENT_EQUALITY(TOP, X, Y) \
    BOOST_CHECK_EQUAL(TOP.x == static_cast<T>(X) && \
                      TOP.y == static_cast<T>(Y), true)

template <typename Point2D>
struct event_processor {
    typedef typename Point2D::coordinate_type coordinate_type;
    typedef typename Point2D::site_event_type site_event_type;
    typedef typename Point2D::circle_event_type circle_event_type;

    event_processor() {}

    void operator()(const site_event_type &site_event) {
        x = site_event.x();
        y = site_event.y();
    }

    void operator()(const circle_event_type &circle_event) {
        x = circle_event.x();
        y = circle_event.y();
    }
        
    coordinate_type x;
    coordinate_type y;
};

BOOST_AUTO_TEST_CASE_TEMPLATE(event_queue_test1, T, test_types) {
    event_processor< point_2d<T> > test_processor;

    event_queue< point_2d<T> > event_q;
    BOOST_CHECK_EQUAL(event_q.empty(), true);
    
    event_q.reset();

    std::vector< point_2d<T> > site_event_vec;
    for (int i = 0; i <= 10; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i);
        site_event_vec.push_back(make_point_2d(x, y));
    }
    sort(site_event_vec.begin(), site_event_vec.end());
    event_q.init(site_event_vec, 0);
    
    event_q.process_top_event(test_processor);
    CHECK_TOP_ELEMENT_EQUALITY(test_processor, 0, 0);
    event_q.pop();

    event_q.process_top_event(test_processor);
    CHECK_TOP_ELEMENT_EQUALITY(test_processor, 1, 1);

    for (int i = 5; i < 10; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i);
        event_q.push(make_circle_event(x, y));
    }

    for (int i = 0; i < 5; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i-1);
        event_q.push(make_circle_event(x, y));
    }
    
    for (int i = 0; i < 10; i++) {
        event_q.process_top_event(test_processor);
        CHECK_TOP_ELEMENT_EQUALITY(test_processor, 1 + i/2, 1 + i/2);
        event_q.pop();
    }

    for (int i = 10; i < 20; i++) {
        event_q.process_top_event(test_processor);
        CHECK_TOP_ELEMENT_EQUALITY(test_processor, 1 + i/2, 1 + (i-1)/2);
        event_q.pop();
    }
    
    BOOST_CHECK_EQUAL(event_q.empty(), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(event_queue_test2, T, test_types) {
    event_processor< point_2d<T> > test_processor;
    event_queue< point_2d<T> > event_q;

    for (int i = 0; i < 10; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i);
        event_q.push(make_circle_event(x, y));
    }

    for (int i = 0; i < 5; i++) {
        T x = static_cast<T>(10-2*i-1);
        T y = static_cast<T>(10-2*i-1);
        event_q.deactivate_event(make_circle_event(x, y));   
    }

    for (int i = 0; i < 5; i++) {
        event_q.process_top_event(test_processor);
        CHECK_TOP_ELEMENT_EQUALITY(test_processor, 2 + 2*i, 2 + 2*i);
        event_q.pop();
    }

    BOOST_CHECK_EQUAL(event_q.empty(), true);
}