// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef BOOST_SOCKET_CONNECTOR_SOCKET_HPP
#define BOOST_SOCKET_CONNECTOR_SOCKET_HPP 1

#include "boost/socket/socket_base.hpp"
#include "boost/socket/data_socket.hpp"
#include "boost/socket/time_value.hpp"
#include "boost/socket/socket_option.hpp"
#include "boost/socket/socket_set.hpp"

namespace boost
{
  namespace socket
  {
    template <typename Protocol, typename Addr>
    data_socket connect(const Protocol& protocol, const Addr& address)
    {
      socket_base socket;
      socket.open(protocol);
      socket.connect(address);
      return data_socket(socket);
    }

    template <typename Protocol, typename Addr>
    data_socket connect(const Protocol& protocol, const Addr& address,
                        const time_value& timeout)
    {
      socket_base socket_to_connect;
      socket_to_connect.open(protocol);
      socket_option_non_blocking non_block(true);
      socket_to_connect.ioctl(non_block);
      SocketError err=socket_to_connect.connect(address);
      if (err==WouldBlock)
      {
        socket_set fdset;
        fdset.add(socket_to_connect.socket());
        int sel=
          ::select(fdset.width(),0,fdset.fdset(),0,
                   (timeval*)timeout.timevalue());
        if (sel==0)
          throw socket_exception();
        if (sel!=1)
        {
          check_error();
          throw socket_exception();
        }
      }
      return data_socket(socket_to_connect);
    }

  }// namespace
}// namespace

#endif
