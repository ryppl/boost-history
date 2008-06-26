// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_TEST_DYNAMIC_MULTI_PORT_HPP
#define DATAFLOW_TEST_DYNAMIC_MULTI_PORT_HPP

#include "my_producer_consumer.hpp"

#include <boost/dataflow/support/dynamic_port.hpp>
#include <vector>

namespace df = boost::dataflow;

struct multi_port_traits : public df::dynamic_port_traits<my_producer>
{};

struct multi_port
    : public df::dynamic_port<multi_port_traits>
{
    multi_port()
    {
        p.resize(2);
    }
    std::vector<my_producer> p;
};

namespace boost { namespace dataflow { namespace extension {

template<>
struct get_dynamic_port_size_impl<multi_port_traits>
{
    typedef size_t result_type;
    
    template<typename Port>
    size_t operator()(Port &port)
    {
        return port.p.size();
    }
};

template<>
struct get_subport_impl<multi_port_traits>
{
    typedef my_producer &result_type;

    template<typename Port>
    result_type operator()(Port &port, size_t n)
    {
        return port.p[n];
    }
};

} } }

#endif
