// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/support.hpp>
#include <boost/dataflow/signals/component/storage.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

void func()
{}

int test_main(int, char* [])
{
    namespace df = boost::dataflow;
    using namespace boost;
    
    BOOST_CHECK((boost::is_same<
        df::result_of::binary_operation<boost::signal<void()>, boost::function<void()>, df::operations::connect, df::signals::tag>::type,
        signals::connection>::type::value));

    BOOST_CHECK((boost::is_same<
        df::result_of::default_binary_operation<boost::signal<void()>, boost::function<void()>, df::operations::connect, df::signals::connect_mechanism, df::signals::tag>::type,
        signals::connection>::type::value));
            
    BOOST_CHECK((boost::is_same<
        df::result_of::default_binary_operation<signals::storage<void ()>, signals::storage<void ()>, df::operations::connect, df::signals::connect_mechanism, df::signals::tag>::type,
        signals::connection>::type::value));

    return 0;
}
