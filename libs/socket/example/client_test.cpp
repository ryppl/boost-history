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

#include "boost/socket/connector_socket.hpp"
#include "boost/socket/ip4.hpp"
#include "boost/socket/socketstream.hpp"

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
  BOOST_MESSAGE("starting");
  socket_base::initialise();

  ip4::address addr;
  addr.port(3234);
  addr.hostname("localhost");
  BOOST_MESSAGE(addr.ip());

  ip4::tcp_protocol protocol;

  try {

    time_value timeout(10,0);
    data_socket socket(connect(protocol,addr,timeout));
    BOOST_CHECK(socket.is_valid());
    BOOST_CHECK(socket.setsockopt(boost::socket::socket_option_linger(1000))
                ==boost::socket::Success);
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
    std::cerr << s.what() << std::endl;
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
