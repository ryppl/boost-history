// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ rpc_example

#include <boost/test/unit_test.hpp>
using boost::unit_test::test_suite;

#include <boost/rpc/server.hpp>
#include <boost/rpc/client.hpp>
#include <boost/rpc/marshal.hpp>
#include <boost/rpc/call.hpp>
#include <boost/rpc/simple_acceptor.hpp>
#include <boost/rpc/simple_connector.hpp>


void nothing()
{
}

int inc(int i)
{
    return ++i;
}

int add2( int i, int j )
{
    return i+j;
}

void inc_inplace(int &i)
{
    ++i;
}

namespace rpc = boost::rpc;
using namespace boost::asio;

void network_rpc_test()
{
    // make a registry keyed on string id-s and using binary_archive serialization
    rpc::registry<std::string> reg;

    // register the functions
    reg.set<void ()>("nothing", nothing);
    reg.set<int (int)>("inc", inc);
    reg.set<int (int, int)>("add2", add2);
    reg.set<void (int &)>("inc_inplace", inc_inplace);

    // create a server for the registry using a simple connection acceptor
    rpc::server<rpc::registry<std::string>, rpc::simple_acceptor> server(reg, 1097);

    // create a client which will connect to the server through the network.
    rpc::connecting_client<rpc::registry<std::string>, rpc::simple_connector> client(
        ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1097));

    // make some function calls
    
    // embed the function id for a void () call
    rpc::call<std::string, void ()> call_nothing("nothing");
    // this call is made asynchrously, nothing will be marshaled back because
    // there are no "out" arguments and the returned handler is not stored.
    client(call_nothing);

    // here, since the returned call handler is stored in an acknowledgement,
    // upon completion the server will marshal back only a confirmation of the completion.
    rpc::acknowledgement_ptr ack = client(call_nothing);
    BOOST_CHECK_NO_THROW(ack->completion().get());

    // embed the function id and parameter for an int (int) call
    rpc::call<std::string, int (int)> call_inc__1("inc",  1);
    // if the returned handler is ignored, nothing is marshaled back:
    client(call_inc__1);

    // if the returned handler is stored in an acknowledgement, only a confirmation of completion
    // is marshaled back.
    ack = client(call_inc__1);
    BOOST_CHECK_NO_THROW(ack->completion().get());

    // if the returned handler is stored in a proper handler, the return value will be marshaled back
    rpc::async_returning_handler<int>::ptr handler_int = client(call_inc__1);
    boost::future<int> future_int(handler_int->return_promise());
    BOOST_CHECK_EQUAL(future_int, 2);

    // handler returners are imlplicitly convertible to futures, which will carry the returned value
    boost::future<int> result_inc = client(call_inc__1);
    BOOST_CHECK_EQUAL(result_inc, 2);

    // handler returners are also convertible to values, which immediately
    // get assigned the value of the return value future, making the call synchronous
    int inced1 = client(call_inc__1);
    BOOST_CHECK_EQUAL(inced1,  2);

    int i = 1;
    // embed the function id and parameters for a void (int &) call
    rpc::call<std::string, void (int &)> call_inc_inplace__i("inc_inplace", i);
    // since the call sends by reference, it will act syncronously - i will immediately be assigned
    // the value of the future int carrying the modified value of the "out" parameter.
    client(call_inc_inplace__i);
    BOOST_CHECK_EQUAL(i, 2);

    // embed the function id and parameter for an int (int, int) call
    rpc::call<std::string, int (int, int)> call_add2__5_6("add2",  5, 6);
    // handler returners are imlplicitly convertible to futures, which will carry the returned value
    boost::future<int> result_add = client(call_add2__5_6);
    BOOST_CHECK_EQUAL(result_add, 11);

    // this call sends the "in" value of an "in/out" paramater through a future (messy)
    boost::promise<int> prom;
    prom.set(200);
    boost::future<int> inced(prom);
    rpc::call<std::string, void (int &)> call_inc_inplace__inced("inc_inplace", inced);
    client(call_inc_inplace__inced);
    BOOST_CHECK_EQUAL(inced, 201);

} // end void network_marshal_test

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE( "RPC test suite" );
    test->add(BOOST_TEST_CASE(&network_rpc_test));
    return test;
}

//]
