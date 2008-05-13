// Copyright 2008 Chintan Rao H.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dsearch.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    boost::dsearch::dsearch<int> object;
    int x = 0;
    BOOST_CHECK_EQUAL(object.get_call_count(),0);
    object.nothing(x,4);
    BOOST_CHECK_EQUAL(object.get_call_count(),1);
    BOOST_CHECK_EQUAL(object.get_nothing_count(),4);
    
    return 0;
};
