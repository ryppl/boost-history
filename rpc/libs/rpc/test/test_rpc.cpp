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

namespace rpc = boost::rpc;
using namespace boost::asio;

class rpc_test_class
{
public:
    void arg_construction()
    {

    }
    void connect()
    {
        // make a registry keyed on int id-s and using binary_archive serialization
        rpc::registry<int> reg;

        // create a server for the registry using a simple connection acceptor
        rpc::server<rpc::registry<int>, rpc::simple_acceptor> server(reg, 1097);

        // create a client which will connect to the server through the network.
        rpc::connecting_client<rpc::registry<int>, rpc::simple_connector> client(
            ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), 1097));
    }
};

test_suite *rpc_test()
{
  test_suite *suite = BOOST_TEST_SUITE("RPC suite");

  boost::shared_ptr<rpc_test_class> instance( new rpc_test_class() );
  suite->add (BOOST_CLASS_TEST_CASE( &rpc_test_class::connect, instance));
  return suite;
}

#define BOOST_TEST_MAIN
boost::unit_test::test_suite* init_unit_test_suite(int, char* [] )
{
  test_suite * test = BOOST_TEST_SUITE("RPC test");
  test->add(rpc_test());
  return test;
}