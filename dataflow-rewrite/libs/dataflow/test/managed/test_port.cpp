// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/dataflow/managed/port.hpp>
#include <boost/dataflow/managed/network.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test ) 
{
    using namespace boost;
    namespace df=boost::dataflow;
    
    BOOST_CHECK((df::has_framework_context<df::managed::framework>::value));

    df::managed::network network;
    df::managed::component input_component(network);
    df::managed::component output_component(network);
    
    df::managed::port<int, df::ports::producer> output(output_component);
    df::managed::port<int, df::ports::consumer> input(input_component);
    
    BOOST_CHECK_EQUAL(output.get(), 0);
    BOOST_CHECK_THROW(input.get(), std::runtime_error);
    
    BOOST_CHECK_EQUAL(network.changed_components().size(), 0u);
    connect(output, input);
    network.update();
    output.set(1);

    BOOST_CHECK_EQUAL(output.get(), 1);
    BOOST_CHECK_EQUAL(input.get(), 1);
    
    BOOST_CHECK_EQUAL(network.changed_components().size(), 1u);
    BOOST_CHECK_EQUAL(*network.changed_components().begin(), &input_component);
    
} // int test_main(int, char* [])



