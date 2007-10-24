// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/support/component_operation.hpp>

bool invoked = false;

struct my_mechanism;

struct my_component_traits
{
    typedef my_mechanism mechanism;
};

struct my_component
{
    typedef my_component_traits component_traits;
};

namespace boost { namespace dataflow { namespace extension {

template<>
struct component_operation_impl<operations::invoke, my_component_traits>
{
    template<typename Component>
    struct apply
    {
        static void call(Component &)
        {
            invoked = true;
        }
    };
};

}}}

#include <boost/test/included/test_exec_monitor.hpp>

namespace df = boost::dataflow;

int test_main(int, char* [])
{
    my_component c;
    
    df::component_operation<df::operations::invoke, my_mechanism>(c);
    BOOST_CHECK(invoked);    
    
    return 0;
} // int test_main(int, char* [])

