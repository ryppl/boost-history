// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/utility/has_call_operator.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

struct no_call_operator
{
};

int test_main(int, char* [])
{
    using namespace boost::dataflow::utility;

    BOOST_CHECK((!has_call_operator<no_call_operator, void()>::type::value));
    
    return 0;
} // int test_main(int, char* [])



