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
#ifndef BOOST_SOCKET_ACCEPTOR_SOCKET_HPP
#define BOOST_SOCKET_ACCEPTOR_SOCKET_HPP 1

#include "boost/socket/socket_base.hpp"
#include "boost/socket/data_socket.hpp"

namespace boost
{
  namespace socket
  {
    //! restricted interfaces using socket_base, but with reduced functionality
    /** The aim here is to provide a restricted programming interface, built
        on the platform neutral socket_base. */

    //! acceptor
    /** interface for a socket that can accept connections. */
    class acceptor_socket
    {
    public:
      template <typename SocketOption>
      int ioctl(SocketOption& option)
      {
        return base_.ioctl(option);
      }

      template <typename SocketOption>
      int getsockopt(SocketOption& option)
      {
        return base_.getsockopt(option);
      }

      template <typename SocketOption>
      int setsockopt(const SocketOption& option)
      {
        return base_.setsockopt(option);
      }

      //! Open a socket for passive connection acceptance
      /** Blocking version */
      template <typename Protocol, class Addr>
      SocketError open(const Protocol& protocol,
                       const Addr& address)
      {
        const SocketError open_error=base_.open(protocol);
        if (open_error!=Success)
          return open_error;

        const SocketError bind_error=base_.bind(address);
        if (bind_error!=Success)
          return bind_error;

        return Success;
      }

      //! Open a socket for passive connection acceptance
      /** Non-blocking version */
      template <typename Protocol, class Addr>
      SocketError open(const Protocol& protocol,
                       const Addr& address,
                       std::size_t backlog)
      {
        const SocketError open_error=base_.open(protocol);
        if (open_error!=Success)
          return open_error;

        const SocketError bind_error=base_.bind(address);
        if (bind_error!=Success)
          return bind_error;

        // set the socket to non-blocking so that listen will succeed
        socket_option_non_blocking non_block(true);
        base_.ioctl(non_block);

        const SocketError listen_error=base_.listen(backlog);
        if (listen_error==Success || listen_error==WouldBlock)
          return Success;
        return listen_error;
      }

      //! accept a connection
      template <class Addr>
      data_socket accept(Addr& address)
      {
        return data_socket(base_.accept(address));
      }


      //! accept a connection
      /** Blocking version */
      template <class Addr>
      socket_base accept(Addr& address, std::size_t backlog)
      {
        const SocketError listen_error=base_.listen(backlog);
        if (listen_error!=Success && listen_error!=WouldBlock)
          return socket_base(INVALID_SOCKET);
        return base_.accept(address);
      }


      //! close a connection
      int close()
      {
        return base_.close();
      }

      //! obtain OS socket
      socket_type socket()
      {
        return base_.socket();
      }

      //! obtain socket_base
      socket_base& base()
      {
        return base_;
      }
    private:
      socket_base base_;
    };

  }// namespace
}// namespace

#endif
