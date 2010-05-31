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
    BOOST_CHECK_EQUAL(TOP.x() == static_cast<T>(X) && \
                      TOP.y() == static_cast<T>(Y), true)

BOOST_AUTO_TEST_CASE_TEMPLATE(event_queue_test1, T, test_types) {
    typedef sweepline_event<T>::kEventType kEventType;

    event_queue< point_2d<T> > event_q;
    BOOST_CHECK_EQUAL(event_q.empty(), true);
    
    event_q.reset();

    std::vector< point_2d<T> > site_event_vec;
    for (int i = 0; i <= 10; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i);
        site_event_vec.push_back(make_point_2d(x, y));
    }
    event_q.init(site_event_vec);
    CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 0, 0);

    event_q.pop();
    CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 1, 1);

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
        CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 1 + i/2, 1 + i/2);
        BOOST_CHECK_EQUAL(event_q.top().get_event_type(),
            (i&1)?(kEventType::CIRCLE_EVENT):(kEventType::SITE_EVENT));
        event_q.pop();
    }

    for (int i = 10; i < 20; i++) {
        CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 1 + i/2, 1 + (i-1)/2);
        BOOST_CHECK_EQUAL(event_q.top().get_event_type(),
            (i&1)?(kEventType::SITE_EVENT):(kEventType::CIRCLE_EVENT));
        event_q.pop();
    }
    
    BOOST_CHECK_EQUAL(event_q.empty(), true);
}