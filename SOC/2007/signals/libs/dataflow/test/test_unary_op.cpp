// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "my_producer_consumer.hpp"
#include <boost/dataflow/support/unary_operation.hpp>

bool disconnected = false;

namespace boost { namespace dataflow { namespace extension {

template<>
struct unary_operation_impl<operations::disconnect_all, my_producer_traits>
{
    template<typename Producer>
    struct apply
    {
        static void call(Producer &)
        {
            disconnected = true;
        }
    };
};

}}}

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int test_main(int, char* [])
{
    my_producer p;
    
    df::unary_operation<df::operations::disconnect_all, my_mechanism, df::ports::producer>(p);
    BOOST_CHECK(disconnected);    
    
    return 0;
} // int test_main(int, char* [])

