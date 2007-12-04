// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/unary_operation.hpp>

bool disconnected = false;

namespace boost { namespace dataflow { namespace extension {

template<>
struct unary_operation_impl<my_producer_traits, operations::disconnect_all>
{
    typedef void result_type;
    
    template<typename Port>
    void operator()(Port &)
    {
        disconnected = true;
    }
};

}}}

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int test_main(int, char* [])
{
    my_producer p;
    my_consumer c;
    
    BOOST_CHECK((df::is_unary_operable<my_producer, df::operations::disconnect_all>::value));
    
    df::unary_operation<df::operations::disconnect_all, df::default_tag>(p);
    BOOST_CHECK(disconnected);    
    
    BOOST_CHECK((!df::is_unary_operable<my_consumer, df::operations::disconnect_all>::value));
    
    return 0;
} // int test_main(int, char* [])

