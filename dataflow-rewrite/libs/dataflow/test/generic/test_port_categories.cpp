// Copyright Stjepan Rajko 2007, 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/generic/port/categories.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;

BOOST_AUTO_TEST_CASE( test ) 
{
    BOOST_CHECK((df::is_port_category<df::ports::producer>::value));
    BOOST_CHECK((df::is_port_category<df::ports::consumer>::value));
    BOOST_CHECK((df::is_port_category<df::ports::producer_consumer>::value));
}