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
    template <typename SocketBase=socket_base<> >
    class data_socket
    {
    public:
      typedef SocketBase socket_base_t;
      typedef typename socket_base_t::socket_t socket_t;
      typedef typename socket_base_t::error_policy error_policy;

      data_socket()
          : m_socket()
      {}

      data_socket(const data_socket& s)
          : m_socket(s.m_socket)
      {}

      explicit data_socket(const socket_base_t& s)
          : m_socket(s)
      {}

      explicit data_socket(socket_t socket)
          : m_socket(socket)
      {}

      // destructor
      ~data_socket() {}


      template <typename SocketOption>
      int ioctl(SocketOption& option)
      {
        return m_socket.ioctl(option);
      }

      template <typename SocketOption>
      int getsockopt(SocketOption& option)
      {
        return m_socket.getsockopt(option);
      }

      template <typename SocketOption>
      int setsockopt(const SocketOption& option)
      {
        return m_socket.setsockopt(option);
      }

      //! receive data
      int recv(void* data, int len)
      {
        return m_socket.recv(data,len);
      }

      //! send data
      /** Returns the number of bytes sent */
      int send(const void* data, int len)
      {
        return m_socket.send(data,len);
      }

      //! shut the socket down
      int shutdown(Direction how=Both)
      {
        return m_socket.shutdown(how);
      }

      //! close the socket
      int close()
      {
        return m_socket.close();
      }

      //! check for a valid socket
      bool is_valid() const
      {
        return m_socket.is_valid();
      }

      //! obtain OS socket
      socket_t socket()
      {
        return m_socket.socket();
      }

      //! obtain a base socket
      socket_base_t& base()
      {
        return m_socket;
      }

      //! compare a socket
      bool operator<(const data_socket& socket) const
      {
        return m_socket<socket.m_socket;
      }

      //! compare a socket
      bool operator==(const data_socket& socket) const
      {
        return m_socket==socket.m_socket;
      }

      //! compare a socket
      bool operator!=(const data_socket& socket) const
      {
        return m_socket!=socket.m_socket;
      }

    private:
      socket_base_t m_socket;
    };

  }
}


#endif
