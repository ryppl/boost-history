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

#ifdef __CYGWIN__
#include "sys/select.h"
#endif

namespace boost
{
  namespace socket
  {
    template <typename SocketBase=socket_base<> >
    struct connector
    {
      typedef SocketBase socket_base_t;
      typedef typename socket_base_t::socket_t socket_t;
      typedef typename socket_base_t::error_policy error_policy;

      template <typename Protocol, typename Addr>
      data_socket<socket_base_t> connect(
        const Protocol& protocol,
        const Addr& address)
      {
        socket_base_t socket;
        socket.open(protocol);
        socket.connect(address);
        return data_socket<socket_base_t>(socket);
      }

      template <typename Protocol, typename Addr>
      data_socket<socket_base_t> connect(
        const Protocol& protocol,
        const Addr& address,
        const time_value& timeout)
      {
        socket_base_t socket_to_connect;
        socket_to_connect.open(protocol);
        option::non_blocking non_block(true);
        int ioctl_err=socket_to_connect.ioctl(non_block);
        int err=socket_to_connect.connect(address);
        if (err==WouldBlock)
        {
          socket_set fdset;
          fdset.insert(socket_to_connect.socket());
          int sel = ::select(fdset.width(), 0, fdset.fdset(), 0,
                             (::timeval*)timeout.timevalue());
          if (sel==-1)
            throw "unexpected select problem";
          else if (sel==0)
            return data_socket<socket_base_t>();
        }
        return data_socket<socket_base_t>(socket_to_connect);
      }

    };

  }// namespace
}// namespace

#endif
