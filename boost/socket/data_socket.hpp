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
#ifndef BOOST_SOCKET_DATA_SOCKET_HPP
#define BOOST_SOCKET_DATA_SOCKET_HPP 1

#include "boost/socket/socket_base.hpp"

namespace boost
{
  namespace socket
  {
    //! restricted interfaces using socket_base, but with reduced functionality
    /** The aim here is to provide a restricted programming interface, built
        on the platform neutral socket_base. */

    //! acceptor
    /** interface for a socket that can accept connections. */
    class data_socket
    {
    public:

      data_socket()
          : socket_()
      {}

      data_socket(const data_socket& s)
          : socket_(s.socket_)
      {}

      explicit data_socket(const socket_base& s)
          : socket_(s)
      {}

      explicit data_socket(socket_type socket)
          : socket_(socket)
      {}

      // destructor
      ~data_socket() {}


      template <typename SocketOption>
      int ioctl(SocketOption& option)
      {
        return socket_.ioctl(option);
      }

      template <typename SocketOption>
      int getsockopt(SocketOption& option)
      {
        return socket_.getsockopt(option);
      }

      template <typename SocketOption>
      int setsockopt(const SocketOption& option)
      {
        return socket_.setsockopt(option);
      }

      //! receive data
      int recv(void* data, int len)
      {
        return socket_.recv(data,len);
      }

      //! send data
      /** Returns the number of bytes sent */
      int send(const void* data, int len)
      {
        return socket_.send(data,len);
      }

      //! shut the socket down
      SocketError shutdown(Direction how=Both)
      {
        return socket_.shutdown(how);
      }

      //! close the socket
      SocketError close()
      {
        return socket_.close();
      }

      //! check for a valid socket
      bool is_valid() const
      {
        return socket_.is_valid();
      }

      //! obtain OS socket
      socket_type socket()
      {
        return socket_.socket();
      }

      //! obtain OS socket
      socket_base& base()
      {
        return socket_;
      }

      //! compare a socket
      bool operator<(const data_socket& socket) const
      {
        return socket_<socket.socket_;
      }

      //! compare a socket
      bool operator==(const data_socket& socket) const
      {
        return socket_==socket.socket_;
      }

      //! compare a socket
      bool operator!=(const data_socket& socket) const
      {
        return socket_!=socket.socket_;
      }

    private:
      socket_base socket_;
    };

  }
}


#endif
