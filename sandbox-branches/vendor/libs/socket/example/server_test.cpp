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
#include "boost/socket/address_info.hpp"
#include "boost/lexical_cast.hpp"

#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

#include "boost/test/unit_test.hpp"
using namespace boost::unit_test_framework;

#ifdef __CYGWIN__
#include "sys/select.h"
#endif

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

using namespace boost::socket;

typedef boost::shared_ptr<data_socket<> > SocketPtr;
typedef std::map<SocketPtr, boost::socket::ip4::address> Clients;

struct socket_equal
  : public std::binary_function<Clients::value_type,
                                socket_base<>::socket_t,
                                bool>
{
  bool operator()(const Clients::value_type& p,
                  socket_base<>::socket_t s) const
  {
    return p.first->socket()==s;
  }
};

void server_test()
{
  try
  {

    BOOST_MESSAGE("starting");

    Clients clients;

    ip4::address addr;
    addr.port(3234);
    addr.ip("127.0.0.1");

    ip4::tcp_protocol protocol;

    option::non_blocking non_block(true);

    typedef acceptor_socket<> acceptor_socket_t;
    typedef acceptor_socket_t::data_connection_t data_connection_t;

    acceptor_socket_t listening_socket;
    BOOST_CHECK(listening_socket.open(protocol,addr,6)==boost::socket::Success);

    boost::socket::socket_set master_set;
    master_set.insert(listening_socket.socket());

    while (true)
    {
      boost::socket::socket_set active_set;
      active_set=master_set;

      BOOST_MESSAGE("Selecting");

      if (::select(active_set.width(),
                   active_set.fdset(),
                   0,
                   0,
                   0)==-1)
      {
        throw "unexpected select result";
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
            SocketPtr accepted_socket(new data_connection_t);
            int ret=listening_socket.accept(*accepted_socket,client);
            if (ret==Success)
            {
              accepted_socket->ioctl(non_block);
              master_set.insert(accepted_socket->socket());
              std::cout << "Accepted client " << client.ip()
                        << ":" << client.port()
                        << std::endl;
              std::string port=boost::lexical_cast<std::string>(client.port());

              address_info_list addr_info(
                client.ip().c_str(),
                port.c_str(),
                address_info_list::canonname);
              address_info_list::iterator iend=addr_info.end();
              for (address_info_list::iterator i=addr_info.begin();
                   i!=iend; ++i)
                std::cout << i->hostname() << std::endl;

              std::cout << accepted_socket->socket() <<std::endl;
              clients.insert(std::make_pair(accepted_socket, client));

            }
            else
              break;
          }
        }
        else
        {
          // these are our data socket
          BOOST_MESSAGE("Receiving data");

          std::string str;

          Clients::iterator client=
            std::find_if( clients.begin(), clients.end(),
                          boost::bind<bool>(socket_equal(),_1,*i));

          BOOST_CHECK(client!=clients.end());

          ip4::address& client_addr=client->second;
          std::cout << client_addr.to_string();
          boost::socket::basic_socket_stream<char> ss(*client->first);

          while (!ss.eof() && !ss.fail())
          {
            ss >> str;
            if (!ss.fail())
              BOOST_MESSAGE(str);
          }
          if (!client->first->is_open())
            BOOST_MESSAGE("Socket closed");

          if (ss.eof())
          {
            // reading from socket should have forced it closed
            BOOST_CHECK(!client->first->is_open());
            master_set.erase(*i);
            clients.erase(client);
          }
        }
      }

    }
  }
  catch (const socket_exception& e)
  {
    std::cerr << e.message()<< std::endl;
    throw;
  }
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
