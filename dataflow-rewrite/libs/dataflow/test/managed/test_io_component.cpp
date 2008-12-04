/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/managed/io_component.hpp>
#include <boost/dataflow/managed/network.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test ) 
{
    using namespace boost;
    using boost::fusion::at_c;
    namespace df=boost::dataflow;
    df::managed::network network;
    df::managed::io_component<bool> c(network);

    BOOST_CHECK_EQUAL(&at_c<0>(df::entities(c)), &c.port<0>());
    BOOST_CHECK_EQUAL(&at_c<1>(df::entities(c)), &c.port<1>());
} // int test_main(int, char* [])



