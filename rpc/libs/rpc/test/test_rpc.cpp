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

    void nothing(rpc::call_options options = rpc::call_options())
    {   client(rpc::call<int, void()> (0), options); }
    int inc(int i, rpc::call_options options = rpc::call_options())
    {   return client(rpc::call<int, int(int)> (1, i), options); }
    int add2(int i, int j, rpc::call_options options = rpc::call_options())
    {   return client(rpc::call<int, int(int, int)> (2, i,j), options); }
    void inc_inplace(int &i, rpc::call_options options = rpc::call_options())
    {   client(rpc::call<int, void(int &)> (3, i), options);    }

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
    // embed the function id and parameters for an int (int, int) call
    rpc::async_call<int, int (int, int)> call2__5_6(2, 5,6);
    // embed the function id and parameters for a void (int &) call
    rpc::call<int, void (int &)> call3__i(3, i);


    rpc::async_call<int, int (int)> acall1__1(1, 1);
    boost::promise<int> prom;
    prom.set(200);
    boost::future<int> inced(prom);
    int j = 100;
    rpc::async_call<int, void (int &)> acall3__j(1, j);
    rpc::async_call<int, void (int &)> acall3__inced(1, inced);


    // make some function calls

    client(call0, rpc::call_options(
        rpc::call_options::no_results));

    client(call0, rpc::call_options(
        rpc::call_options::completed_only));

    BOOST_CHECK(call0.has_completed());

    client(call1__1, rpc::call_options(
        rpc::call_options::return_only));

    BOOST_CHECK(call1__1.has_completed());
    BOOST_CHECK_EQUAL(call1__1.return_value(), 2);

    client(call3__i, rpc::call_options(
        rpc::call_options::all_out_parameters));

    BOOST_CHECK(call3__i.has_completed());
    BOOST_CHECK_EQUAL(i, 2);

    client(call2__5_6, rpc::call_options(
        rpc::call_options::return_only));

    while(!call2__5_6.has_completed())
    {}

    BOOST_CHECK_EQUAL(call2__5_6.return_value(), 11);

    boost::future<int> inced1 = client(acall1__1, rpc::call_options(
        rpc::call_options::return_only));

    BOOST_CHECK_EQUAL(inced1.get(),  2);

    remote_interface<rpc::registry<int> > remote(client);

    remote.nothing();

    BOOST_CHECK_EQUAL(remote.inc(5), 6);
    BOOST_CHECK_EQUAL(remote.add2(10, 20), 30);
    int x=20;
    remote.inc_inplace(x);
    BOOST_CHECK_EQUAL(x, 21);

    client(acall3__inced, rpc::call_options(
        rpc::call_options::all_out_parameters));
    BOOST_CHECK_EQUAL(inced, 201);

    client(acall3__j, rpc::call_options(
        rpc::call_options::all_out_parameters));
    BOOST_CHECK_EQUAL(j, 101);
} // end void network_marshal_test

test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test_suite* test = BOOST_TEST_SUITE( "Utility test suite" );
    test->add(BOOST_TEST_CASE(&network_marshal_test));
    return test;
}
