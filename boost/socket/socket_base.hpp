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
#include "boost/socket/impl/default_socket_impl.hpp"
#include "boost/socket/impl/default_error_policy.hpp"
#include "boost/socket/socket_errors.hpp"
#include "boost/socket/concept/protocol.hpp"
#include "boost/socket/concept/address.hpp"

#include "boost/concept_check.hpp"

namespace boost
{
  namespace socket
  {

    //! platform independent, low level interface
    /** Implementation will depend on platform */
    template <typename ErrorPolicy=default_error_policy,
              typename SocketImpl=impl::default_socket_impl>
    class socket_base
    {
    public:
      typedef ErrorPolicy error_policy;
      typedef SocketImpl socket_impl;
      typedef typename socket_impl::socket_t socket_t;
      typedef socket_base<error_policy,socket_impl> self_t;

      socket_base()
          : m_socket_impl()
      {}

      socket_base(const socket_base& s)
          : m_socket_impl(s.m_socket_impl),
            m_error_policy(s.m_error_policy)
      {}

      explicit socket_base(socket_t socket)
          : m_socket_impl(socket)
      {}

      ~socket_base()
      {}

      template <typename SocketOption>
      int ioctl(SocketOption& option)
      {
        int ret = m_socket_impl.ioctl(option.optname(), option.data());
        if (ret!=Success)
          return m_error_policy.handle_error(function::ioctl,ret);
        return Success;
      }

      template <typename SocketOption>
      int getsockopt(SocketOption& option)
      {
        BOOST_STATIC_ASSERT(option.can_get);
        int len=option.size();
        int ret = ::getsockopt(socket_,
                               option.level, option.option, &option.value,
                               &len);
        if (ret!=Success)
          return m_error_policy.handle_error(function::getsockopt,ret);
        return Success;
      }

      template <typename SocketOption>
      int setsockopt(const SocketOption& option)
      {
        BOOST_STATIC_ASSERT(option.can_set);
        int ret = m_socket_impl.setsockopt(option.level(),
                                           option.optname(),
                                           option.data(),
                                           option.size());
        if (ret!=Success)
          return m_error_policy.handle_error(function::setsockopt,ret);
        return Success;
      }

      // create a socket, Address family, type {SOCK_STREAM, SOCK_DGRAM },
      // protocol is af specific
      template <typename Protocol>
      int open(const Protocol& protocol)
      {
        boost::function_requires< ProtocolConcept<Protocol> >();

        // SOCKET socket(int af,int type,int protocol);
        int ret = m_socket_impl.open(protocol.family(),
                                     protocol.type(),
                                     protocol.protocol());
        if (ret!=Success)
          return m_error_policy.handle_error(function::open,ret);
        return Success;
      }

      template <class Addr>
      int connect(const Addr& address)
      {
        boost::function_requires< AddressConcept<Addr> >();
        int ret= m_socket_impl.connect(address.representation());
        if (ret!=Success)
          return m_error_policy.handle_error(function::connect,ret);
        return Success;
      }

      template <class Addr>
      int bind(const Addr& address)
      {
        int ret=m_socket_impl.bind(address.representation());
        if (ret!=Success)
          return m_error_policy.handle_error(function::bind,ret);
        return Success;
      }

      int listen(int backlog)
      {
        int ret=m_socket_impl.listen(backlog);
        if (ret!=Success)
          return m_error_policy.handle_error(function::listen,ret);
        return Success;
     }

      //! accept a connection
      template <class Addr>
      std::pair<self_t,int> accept(Addr& address)
      {
        boost::function_requires< AddressConcept<Addr> >();
        std::pair<void*,size_t> rep=address.representation();
        std::pair<socket_impl,int> ret;
        ret = m_socket_impl.accept(rep);
        if (ret.second!=Success)
        {
          int ret2=m_error_policy.handle_error(function::accept,ret.second);
          return std::make_pair(self_t(),ret2);
        }
        return std::make_pair(self_t(ret.first),Success);
      }

      //! receive data
      int recv(void* data, size_t len)
      {
        int ret=m_socket_impl.recv(data, len);
        if (ret<0)
          return m_error_policy.handle_error(function::recv,ret);
        return ret;
      }

      //! send data
      /** Returns the number of bytes sent */
      int send(const void* data, size_t len)
      {
        int ret=m_socket_impl.send(data, len);
        if (ret<0)
          return m_error_policy.handle_error(function::send,ret);
        return ret;
      }

      //! shut the socket down
      int shutdown(Direction how=Both)
      {
        int ret = m_socket_impl.shutdown(how);
        if (ret!=Success)
          return m_error_policy.handle_error(function::shutdown,ret);
        return Success;
      }

      //! close the socket
      int close()
      {
        int ret=m_socket_impl.close();
        if (ret!=Success)
          return m_error_policy.handle_error(function::close, ret );
        return Success;
     }

      //! check for a valid socket
      bool is_valid() const
      {
        return m_socket_impl.is_valid();
      }

      //! obtain OS socket
      socket_t socket()
      {
        return m_socket_impl.socket();
      }

      //! obtain OS socket
      const socket_t socket() const
      {
        return m_socket_impl.socket();
      }

      //! compare a socket
      bool operator<(const socket_base& socket) const
      {
        return m_socket_impl<socket.m_socket_impl;
      }

      //! compare a socket
      bool operator==(const socket_base& socket) const
      {
        return m_socket_impl==socket.m_socket_impl;
      }

      //! compare a socket
      bool operator!=(const socket_base& socket) const
      {
        return m_socket_impl!=socket.m_socket_impl;
      }

    private:
      socket_base(const socket_impl& s)
          : m_socket_impl(s)
      {}

      socket_impl m_socket_impl;
      error_policy m_error_policy;
    };


  }
}

#endif
