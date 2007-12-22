// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/fusion_vector_port.hpp>
#include <boost/fusion/include/vector.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;
namespace fusion = boost::fusion;

typedef fusion::vector<my_producer &, my_consumer &> vector_type;
        
struct multi_port
    : public df::fusion_vector_port<df::ports::producer,vector_type>
{
    multi_port() : df::fusion_vector_port<df::ports::producer,vector_type>(
        vector_type(p, c)) {}
    
    my_producer p;
    my_consumer c;
};

int test_main(int, char* [])
{
    multi_port mp;
    
    BOOST_CHECK_EQUAL(&mp.p, &df::get_port_c<0>(mp));
    BOOST_CHECK_EQUAL(&mp.c, &df::get_port_c<1>(mp));
    
    return 0;    
} // int test_main(int, char* [])
