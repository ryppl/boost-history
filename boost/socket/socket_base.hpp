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
#ifndef BOOST_SOCKET_SOCKET_BASE_HPP
#define BOOST_SOCKET_SOCKET_BASE_HPP 1

#include "boost/socket/config.hpp"

#if defined(USES_WINSOCK2)
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif


#include "boost/socket/address.hpp"
#include "boost/socket/protocol.hpp"
#include "boost/socket/time_value.hpp"
#include "boost/socket/socket_option.hpp"
#include "boost/socket/socket_errors.hpp"

#include "boost/config.hpp"
#include "boost/static_assert.hpp"
#include "boost/concept_check.hpp"

namespace boost
{
  namespace socket
  {

#if defined(USES_WINSOCK2)
    enum Direction
    {
      Receiving=SD_RECEIVE,
      Sending=SD_SEND,
      Both=SD_BOTH
    };

#else
    enum Direction
    {
#ifdef SHUT_RD
      Receiving=SHUT_RD,
#else
      Receiving=0,
#endif

#ifdef SHUT_WR
      Sending=SHUT_WR,
#else
      Sending=1,
#endif

#ifdef SHUT_RDWR
      Both=SHUT_RDWR
#else
      Both=2
#endif
    };
#endif

    //! platform independent, low level interface
    /** Implementation will depend on platform */
    class socket_base
    {
    public:

      socket_base()
          : socket_(invalid_socket)
      {}

      socket_base(const socket_base& s)
          : socket_(s.socket_)
      {}

      explicit socket_base(socket_type socket)
          : socket_(socket)
      {}

      // destructor
      ~socket_base() {}


      template <typename SocketOption>
      int ioctl(SocketOption& option)
      {
        BOOST_STATIC_ASSERT(option.level==SOL_IOCTL);
#ifdef USES_WINSOCK2
        int ret = ::ioctlsocket(socket_,
                                option.option,
                                &option.value);
#else
        int ret = ::ioctl(socket_,
                          option.option,
                          &option.value);
#endif
        if (ret==socket_error)
          ret=check_error();
        return ret;
      }

      template <typename SocketOption>
      int getsockopt(SocketOption& option)
      {
        BOOST_STATIC_ASSERT(option.level!=SOL_IOCTL);
        BOOST_STATIC_ASSERT(option.can_get);
        int len=option.size();
        int ret = ::getsockopt(socket_,
                               option.level, option.option, &option.value,
                               &len);
        if (ret==socket_error)
          check_error();
        return ret;
      }

      template <typename SocketOption>
      int setsockopt(const SocketOption& option)
      {
        BOOST_STATIC_ASSERT(option.level!=SOL_IOCTL);
        BOOST_STATIC_ASSERT(option.can_set);
        int ret = ::setsockopt(socket_,
                               option.level,
                               option.option,
                               (char*)&option.value,
                               option.size());
        if (ret==socket_error)
          check_error();
        return ret;
      }

      // create a socket, Address family, type {SOCK_STREAM, SOCK_DGRAM },
      // protocol is af specific
      template <typename Protocol>
      SocketError open(const Protocol& protocol)
      {
        boost::function_requires< ProtocolConcept<Protocol> >();

        // SOCKET socket(int af,int type,int protocol);
        socket_ = ::socket(protocol.family(),
                           protocol.type(),
                           protocol.protocol());
        if (socket_==invalid_socket)
        {
          SocketError err=check_error();
          return err;
        }
        return Success;
      }

      template <class Addr>
      SocketError connect(const Addr& address)
      {
        boost::function_requires< AddressConcept<Addr> >();
        int ret= ::connect(socket_,
                           address.socket_address(),
                           address.size());
        if (ret!=Success)
          return check_error();
        return Success;
      }

      template <class Addr>
      SocketError bind(const Addr& address)
      {
        int ret=::bind(socket_,
                       address.socket_address(),
                       address.size());
        if (ret!=Success)
          return check_error();
        return Success;
      }

      SocketError listen(int backlog)
      {
        int ret=::listen(socket_, backlog);
        if (ret!=0)
          return check_error();
        return Success;
     }

      //! accept a connection
      template <class Addr>
      socket_base accept(Addr& address)
      {
        boost::function_requires< AddressConcept<Addr> >();

        size_type len=address.size();
        socket_type new_socket
          = ::accept(socket_, address.socket_address(), &len);
        if (new_socket==invalid_socket)
        {
          /*int ret=*/check_error();
        }
        return socket_base(new_socket);
      }

      //! receive data
      int recv(void* data, size_type len)
      {
        int flags = 0;
        int ret=::recv(socket_, (char*)data, len,flags);
        return ret;
      }

      //! send data
      /** Returns the number of bytes sent */
      int send(const void* data, size_type len)
      {
        int flags = 0;
        int ret=::send(socket_, (const char*)data, len, flags);
        return ret;
      }

      //! shut the socket down
      SocketError shutdown(Direction how=Both)
      {
        int ret = ::shutdown(socket_, static_cast<int>(how));
        if (ret!=Success)
          return check_error();
        return Success;
      }

      //! close the socket
      SocketError close()
      {
#if defined(USES_WINSOCK2)
        int ret = ::closesocket(socket_);
#else
        int ret = ::close(socket_);
#endif
        if (ret!=Success)
          return check_error();

        socket_=invalid_socket;

        return Success;
     }

      //! check for a valid socket
      bool is_valid() const
      {
        return socket_!=invalid_socket;
      }

      //! obtain OS socket
      socket_type socket()
      {
        return socket_;
      }

      //! obtain OS socket
      const socket_type socket() const
      {
        return socket_;
      }

      //! compare a socket
      bool operator<(const socket_base& socket) const
      {
        return socket_<socket.socket_;
      }

      //! compare a socket
      bool operator==(const socket_base& socket) const
      {
        return socket_==socket.socket_;
      }

      //! compare a socket
      bool operator!=(const socket_base& socket) const
      {
        return socket_!=socket.socket_;
      }

      //! initialise socket system
      static inline bool initialise();
      //! finalise socket system
      static inline void finalise();
    private:
      socket_type socket_;
    };

#ifdef _WIN32
    bool socket_base::initialise()
    {
      WORD wVersionRequested;
      WSADATA wsaData;
      int err;

      wVersionRequested = ((MAKEWORD(2, 2)));

      err = WSAStartup(wVersionRequested, &wsaData);
      if (err != 0)
        return true;
      if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
      {
        WSACleanup();
        return true;
      }
      return false;
    }

    void socket_base::finalise()
    {
      WSACleanup();
    }
#else
    bool socket_base::initialise()
    {
      return true;
    }

    void socket_base::finalise()
    {
    }
#endif

  }
}

#endif
