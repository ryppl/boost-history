// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

template<typename Registry>
class remote_interface
{
public:
    remote_interface(rpc::client<Registry> &client) : client(client) {};

    boost::future<void> nothing()
    {   
        return client(rpc::call<int, void()> (0));
    }
    boost::future<int> inc(int i)
    {   
        return client(rpc::call<int, int(int)> (1, i));
    }
    boost::future<int> add2(int i, int j)
    {   
        return client(rpc::call<int, int(int, int)> (2, i,j));
    }
    boost::future<void> inc_inplace(int &i)
    {
        return client(rpc::call<int, void(int &)> (3, i));
    }
private:
    rpc::client<Registry> &client;
};

void network_marshal_test()
{
    // make a registry keyed on int id-s and using binary_archive serialization
    rpc::registry<int> reg;

    // register the functions
    reg.set<void ()>(0, nothing);
    reg.set<int (int)>(1, inc);
    reg.set<int (int, int)>(2, add2);
    reg.set<void (int &)>(3, inc_inplace);

    // create a server for the registry using a simple connection acceptor
    rpc::server<rpc::registry<int>, rpc::simple_acceptor> server(reg, 1097);

    // create a client which will connect to the server through the network.
    rpc::connecting_client<rpc::registry<int>, rpc::simple_connector> client(
        ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1097));

    // prepare some function calls
    int i = 1;
    // embed the function id for a void () call
    rpc::call<int, void ()> call0(0);
    // embed the function id and parameter for an int (int) call
    rpc::call<int, int (int)> call1__1(1,  1);
    // embed the function id and parameter for an int (int, int) call
    rpc::call<int, int (int, int)> call2__5_6(2,  5, 6);
    // embed the function id and parameters for a void (int &) call
    rpc::call<int, void (int &)> call3__i(3, i);

    boost::promise<int> prom;
    prom.set(200);
    boost::future<int> inced(prom);
    int j = 100;
    rpc::call<int, void (int &)> call3__j(1, j);
    rpc::call<int, void (int &)> call3__inced(1, inced);


    // make some function calls
    
    // this call is made asynchrously, nothing will be marshaled back
    client(call0);

    // this call will return a handler that we can use check for completion
    rpc::acknowledgement_ptr handler = client(call0);
    BOOST_CHECK_NO_THROW(handler->completion().get());

    // this call will return a handler that can be used to get the return value
    // since the call includes an "out" parameter, everything gets marshalled
    // back no matter what.
    rpc::async_returning_handler<int>::ptr handler_int = client(call1__1);
    boost::future<int> future_int(handler_int->return_promise());
    BOOST_CHECK_EQUAL(future_int, 2);

    // since the call sends by reference, it will act syncronously
    client(call3__i);
    BOOST_CHECK_EQUAL(i, 2);

    // handler returners are imlplicitly convertible to futures
    boost::future<int> result = client(call2__5_6);

    BOOST_CHECK_EQUAL(result, 11);

    // and values, making the call synchronous
    int inced1 = client(call1__1);
    BOOST_CHECK_EQUAL(inced1,  2);

    // this call sends the value through a feature (messy)
    client(call3__inced);
    BOOST_CHECK_EQUAL(inced, 201);

    // another synchronous call
    client(call3__j);
    BOOST_CHECK_EQUAL(j, 101);

    // make calls using the interface class...
    remote_interface<rpc::registry<int> > remote(client);

    remote.nothing();

    BOOST_CHECK_EQUAL(remote.inc(5), 6);
    BOOST_CHECK_EQUAL(remote.add2(10, 20), 30);
    int x=20;
    remote.inc_inplace(x);
    BOOST_CHECK_EQUAL(x, 21);

} // end void network_marshal_test

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE( "Utility test suite" );
    test->add(BOOST_TEST_CASE(&network_marshal_test));
    return test;
}
