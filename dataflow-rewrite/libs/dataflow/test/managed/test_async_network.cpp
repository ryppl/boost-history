// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/managed/port.hpp>
#include <boost/dataflow/managed/network.hpp>
#include <boost/asio/io_service.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace df=boost::dataflow;


BOOST_AUTO_TEST_CASE( test ) 
{
    // our network
    df::managed::network network(boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service()));
    
    df::managed::component c(network);
    df::managed::port<int, df::ports::producer> p(c);
    p.set(0);

    network.async_set_port_value(p, 1);
    
    BOOST_CHECK_EQUAL(p.get(), 0);
    
    network.io_service().run();
    
    BOOST_CHECK_EQUAL(p.get(), 1);
} // int test_main(int, char* [])



