/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/managed/fusion_component.hpp>
#include <boost/dataflow/managed/network.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

int test_main(int, char* [])
{
    using namespace boost;
    namespace df=boost::dataflow;
    df::managed::network network;
    df::managed::fusion_component<bool> c(network);

    BOOST_CHECK_EQUAL((&df::get_port<boost::mpl::int_<0> >(c)), &c.port<0>());
    BOOST_CHECK_EQUAL((&df::get_port<boost::mpl::int_<1> >(c)), &c.port<1>());

    return 0;
} // int test_main(int, char* [])



