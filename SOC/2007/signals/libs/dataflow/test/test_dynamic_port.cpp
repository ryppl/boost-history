// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "dynamic_multi_port.hpp"

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    multi_port mp;
    BOOST_CHECK((df::is_dynamic_port<multi_port>::value));
    BOOST_CHECK_EQUAL(df::get_dynamic_port_size<df::default_tag>(mp),2u);
    BOOST_CHECK_EQUAL((&df::get_subport<df::default_tag>(mp, 0)),&mp.p[0]);
    
    return 0;  
} // int test_main(int, char* [])
