// Boost sweepline library event_queue_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <cmath>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE event_queue_test
#include <boost/test/test_case_template.hpp>

#define CHECK_TOP_ELEMENT_EQUALITY(TOP, X, Y) \
    BOOST_CHECK_EQUAL(TOP.lower_x() == static_cast<T>(X) && \
                      TOP.y() == static_cast<T>(Y), true)

BOOST_AUTO_TEST_CASE_TEMPLATE(event_queue_test1, T, test_types) {
    detail::circle_events_queue<T> event_q;
    BOOST_CHECK_EQUAL(event_q.empty(), true);

    event_q.clear();
    for (int i = 0; i < 10; i++) {
        T x = static_cast<T>(-i);
        T y = static_cast<T>(10-i);
        event_q.push(detail::make_circle_event<T>(x, y, x + static_cast<T>(10)));
    }

    for (int i = 0; i < 10; i++) {
        CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 1 + i, 1 + i);
        event_q.pop();
    }

    BOOST_CHECK_EQUAL(event_q.empty(), true);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(event_queue_test2, T, test_types) {
    typedef detail::circle_event<T> circle_event_type;
    detail::circle_events_queue<T> event_q;
    detail::site_event<T> temp_site =
        detail::make_site_event<T>(static_cast<T>(0), static_cast<T>(0), 0);

    for (int i = 0; i < 10; i++) {
        T x = static_cast<T>(10-i);
        T y = static_cast<T>(10-i);
        circle_event_type c = detail::make_circle_event<T>(x, y, x);
        if (i&1)
            event_q.push(c)->deactivate();
        else
            event_q.push(c);
    }

    for (int i = 0; i < 5; i++) {
        CHECK_TOP_ELEMENT_EQUALITY(event_q.top(), 2 + 2*i, 2 + 2*i);
        event_q.pop();
    }

    BOOST_CHECK_EQUAL(event_q.empty(), true);
}
