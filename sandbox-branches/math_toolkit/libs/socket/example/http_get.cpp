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


#include "boost/socket/address_info.hpp"
#include "boost/socket/any_protocol.hpp"
#include "boost/socket/any_address.hpp"
#include "boost/socket/ip4/address.hpp"

#include "boost/socket/connector_socket.hpp"
#include "boost/socket/socketstream.hpp"

#include "boost/socket/socket_exception.hpp"

//#define SPIRIT_DEBUG
#include "url.hpp"


#include <iostream>

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif


int main(int argc, const char** argv)
{
  if (argc!=2)
  {
    std::cerr << "usage: http_get url" << std::endl;
    return 1;
  }

  UrlGrammar grammar;
  SPIRIT_DEBUG_RULE(grammar);

  spirit::parse_info<const char*> info
    =spirit::parse((char const*)argv[1], grammar);

  if (schemename!="http")
  {
    std::cerr << schemename << " not supported" << std::endl;
    return 1;
  }

  boost::socket::address_info_list list(servername.c_str(), "http");

  boost::socket::address_info_list::iterator i=list.begin(), iend=list.end();
  if (i==iend)
  {
    std::cerr << "host " << servername << " not found\n";
    return 2;
  }
  std::cout << std::distance(i,iend) << " addresses found\n";

  boost::socket::connector<> connector;
  boost::socket::connector<>::data_connection_t connection;

  try
  {
    connector.connect(connection, i->protocol(), i->address());
    boost::socket::basic_socket_stream<char> s(connection);

    s << "GET " << pathname << " HTTP/1.0\r\n\r\n";
    s.flush();

    std::string result;
    s >> result;
    while (!s.eof())
    {
      std::cout << result << " ";
      s >> result;
    }

    if (connection.is_open())
      std::cerr << "\n\nData connection was not closed properly" << std::endl;

  }
  catch (boost::socket::socket_exception& e)
  {
    std::cerr << e.message() << std::endl;
  }

  return 0;
}


#ifdef _MSC_VER
#pragma warning (pop)
#endif
