// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma warning (disable: 4786 4305)
  // 4786 truncated debug symbolic name
  // 4305 truncation from const double to float
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif

#include "boost/socket/ip4.hpp"
#include "boost/socket/socketstream.hpp"
#include "boost/socket/socket_exception.hpp"
#include "boost/socket/connector_socket.hpp"

#include <iostream>
#include <cassert>

#include "boost/test/unit_test.hpp"
using namespace boost::unit_test_framework;

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

using namespace boost::socket;

void client_test()
{
  try
  {

    BOOST_MESSAGE("starting");

    ip4::address addr;
    addr.port(3234);
    addr.ip("127.0.0.1");
    BOOST_MESSAGE(addr.ip());

    ip4::tcp_protocol protocol;

    typedef connector<> connector_t;
    typedef connector_t::data_connection_t data_connection_t;

    connector_t connector;

    boost::socket::time_t timeout(0,0,10,0);
    data_connection_t socket;
    BOOST_CHECK(connector.connect(socket,protocol,addr)==boost::socket::Success);
    BOOST_CHECK(socket.is_open());
//     BOOST_CHECK(socket.setsockopt(boost::socket::option::linger(1000))
//                 ==boost::socket::Success);
    BOOST_MESSAGE("Connected");

    boost::socket::basic_socket_stream<char> s(socket);
    for (int k=0;k<10;++k)
    {
      s << "hello\n";
      s << "hello ";
      s.flush();
    }

    BOOST_CHECK(socket.close()==boost::socket::Success);
  }
  catch (const socket_exception& s)
  {
    std::cerr << s.message() << std::endl;
  }

}


//----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int , char*[] )
{
  test_suite* test= BOOST_TEST_SUITE("socket test");
  test->add( BOOST_TEST_CASE( client_test ));
  return test;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
