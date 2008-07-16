// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/container/lazy_sequence.hpp>
#include <boost/fusion/include/at.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

int one(int index)
{
    return 1;
}

int test_main(int, char* [])
{
    using namespace boost::fusion;
    
    vector<int, float, double> ones(make_lazy_sequence(one, boost::mpl::int_<3>()));

    BOOST_CHECK_EQUAL(at_c<0>(ones), 1);
    BOOST_CHECK_EQUAL(at_c<1>(ones), 1.0f);
    BOOST_CHECK_EQUAL(at_c<2>(ones), 1.0);
        
    return 0;
} // int test_main(int, char* [])




