// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/utility/forced_sequence.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    using namespace boost;
    namespace df=boost::dataflow;

    BOOST_CHECK((is_same<df::utility::forced_sequence<void>::type, mpl::vector<> >::value));
    BOOST_CHECK((is_same<df::utility::forced_sequence<int>::type, mpl::vector<int> >::value));
    BOOST_CHECK((is_same<df::utility::forced_sequence<mpl::vector<int> >::type, mpl::vector<int> >::value));
    
    return 0;
} // int test_main(int, char* [])



