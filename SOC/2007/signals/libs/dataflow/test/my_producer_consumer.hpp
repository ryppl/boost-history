// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_TEST_MY_PRODUCER_CONSUMER_HPP
#define DATAFLOW_TEST_MY_PRODUCER_CONSUMER_HPP

#include <boost/dataflow/support/port.hpp>


namespace df = boost::dataflow;

struct incomplete;
struct empty {};

struct my_mechanism;

struct my_producer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::producer,
        df::concepts::producer>
{};

struct my_consumer_traits
    : public df::port_traits<
        my_mechanism,
        df::ports::consumer,
        df::concepts::consumer>
{};

struct my_producer : public df::port<my_producer_traits>
{};

struct my_consumer : public df::port<my_consumer_traits>
{};

#endif
