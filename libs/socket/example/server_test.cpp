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
#include "boost/socket/acceptor_socket.hpp"
#include "boost/socket/socket_set.hpp"

#include <iostream>
#include <vector>
#include <map>

#include "boost/test/unit_test.hpp"
using namespace boost::unit_test_framework;

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

using namespace boost::socket;

typedef std::map<data_socket, boost::socket::ip4::address> Clients;

void server_test()
{
  BOOST_MESSAGE("starting");
  socket_base::initialise();

  Clients clients;

  ip4::address addr;
  addr.port(3234);
  addr.hostname("localhost");

  ip4::tcp_protocol protocol;

  socket_option_non_blocking non_block(true);

  acceptor_socket listening_socket;
  BOOST_CHECK(listening_socket.open(protocol,addr,6)==boost::socket::Success);

  boost::socket::socket_set master_set;
  master_set.add(listening_socket.socket());

  while (true)
  {
    boost::socket::socket_set active_set;
    active_set=master_set;

    BOOST_MESSAGE("Selecting");

    if (::select(active_set.width(),
                 active_set.fdset(),
                 0,
                 0,
                 0)==socket_error)
    {
      check_error();
      return;
    }

    const boost::socket::socket_set::iterator
      i_end=active_set.end();

    for (boost::socket::socket_set::iterator i=active_set.begin();
         i!=i_end; ++i)
    {
      if (*i==listening_socket.socket())
      {
        //! this is our acceptor socket
        while (true)
        {
          ip4::address client;
          data_socket accepted_socket=listening_socket.accept(client);
          if (accepted_socket.is_valid())
          {
            BOOST_CHECK(accepted_socket.ioctl(non_block)==boost::socket::Success);
            master_set.add(accepted_socket.socket());
            BOOST_MESSAGE("Accepted client");
            BOOST_MESSAGE(client.ip());
            BOOST_MESSAGE(client.hostname());
            std::cout << accepted_socket.socket() <<std::endl;
            clients.insert(std::make_pair(accepted_socket, client));

          }
          else
            break;
        }
        ++i;
      }
      else
      {
        // these are our data socket
        BOOST_MESSAGE("Receiving data");

        std::string str;
        data_socket data_socket(*i);

        ip4::address& client=clients[data_socket];
        BOOST_MESSAGE(client.hostname());
        boost::socket::basic_socket_stream<char> ss(data_socket);

        while (!ss.eof() && !ss.fail())
        {
          ss >> str;
          if (!ss.fail())
            BOOST_MESSAGE(str);
        }
        if (ss.eof())
        {
          master_set.remove(data_socket.socket());
          clients.erase(data_socket);
          BOOST_CHECK(data_socket.close()==boost::socket::Success);
        }
      }
    }

  }

  BOOST_CHECK(listening_socket.close()==boost::socket::Success);
}


//----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int , char*[] )
{
  test_suite* test= BOOST_TEST_SUITE("socket test");
  test->add( BOOST_TEST_CASE( server_test ));
  return test;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
