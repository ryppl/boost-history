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
#ifndef BOOST_SOCKET_SOCKET_OPTION_HPP
#define BOOST_SOCKET_SOCKET_OPTION_HPP 1

#include "boost/socket/config.hpp"

#if defined(USES_WINSOCK2)
#include <Winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#endif

#include "boost/config.hpp"

namespace boost
{
  namespace socket
  {
    template <int OptionValue, typename ValueType>
    struct ioctl_option
    {
      ioctl_option(const ValueType& ov)
          : value(ov)
      {}

      BOOST_STATIC_CONSTANT(int, option = OptionValue);
      ValueType value;
      static int size() {return sizeof(ValueType); }
    };

    template <int LevelValue, int OptionValue, bool CanGet, bool CanSet,
              typename ValueType>
    struct socket_option
    {
      socket_option() {}
      socket_option(const ValueType& ov)
          : value(ov)
      {}

      BOOST_STATIC_CONSTANT(int, level = LevelValue);
      BOOST_STATIC_CONSTANT(int, option = OptionValue);
      BOOST_STATIC_CONSTANT(bool, can_get = CanGet);
      BOOST_STATIC_CONSTANT(bool, can_set = CanSet);
      ValueType value;
      static int size() {return sizeof(ValueType); }
    };

    struct linger : public ::linger
    {
      linger()
      {
        l_onoff=false;
      }

      linger(u_short time_sec)
      {
        l_onoff=true;
        l_linger=time_sec;
      }
    };

#ifdef _WIN32
    typedef BOOL bool_t;
#else
    typedef bool bool_t;
#endif

    #define SOL_IOCTL 99
    //! non-blocking
    typedef socket_option<SOL_IOCTL, FIONBIO, false, true, u_long>
      socket_option_non_blocking;
    //! amount of data that can be read
    typedef socket_option<SOL_IOCTL, FIONREAD, false, true, u_long>
      socket_option_data_pending;
    //! all OOB data read
    typedef socket_option<SOL_IOCTL, SIOCATMARK, false, true, u_long>
      socket_option_at_mark;

    typedef socket_option<SOL_SOCKET, SO_ACCEPTCONN, true, false, bool>
      socket_option_listening;
    typedef socket_option<SOL_SOCKET, SO_BROADCAST, true, true, bool>
      socket_option_broadcast;

#if defined(USES_WINSOCK2) && defined(_MSC_VER) && _MSC_VER > 1200
    typedef socket_option<SOL_SOCKET, SO_CONDITIONAL_ACCEPT, true, true, bool>
      socket_option_conditional_accept;
#endif
    typedef socket_option<SOL_SOCKET, SO_DEBUG, true, true, bool>
      socket_option_debug;
    typedef socket_option<SOL_SOCKET, SO_DONTLINGER, true, true, bool>
      socket_option_dontlinger;
    typedef socket_option<SOL_SOCKET, SO_DONTROUTE, true, true, bool>
      socket_option_dontroute;
    typedef socket_option<SOL_SOCKET, SO_ERROR, true, false, bool>
      socket_option_error;
#if defined(USES_WINSOCK2)
    typedef socket_option<SOL_SOCKET, SO_GROUP_ID, true, false, GROUP>
      socket_option_group_id;
    typedef socket_option<SOL_SOCKET, SO_GROUP_PRIORITY, true, true, int>
      socket_option_group_priority;
#endif
    typedef socket_option<SOL_SOCKET, SO_KEEPALIVE, true, true, bool>
      socket_option_keepalive;
    typedef socket_option<SOL_SOCKET, SO_LINGER, true, true, linger>
      socket_option_linger;
#if defined(USES_WINSOCK2)
    typedef socket_option<SOL_SOCKET, SO_MAX_MSG_SIZE, true,false,unsigned int>
      socket_option_max_msg_size;
#endif
    typedef socket_option<SOL_SOCKET, SO_OOBINLINE, true, true, bool>
      socket_option_oobinline;
#if defined(USES_WINSOCK2)
    typedef socket_option<SOL_SOCKET, SO_PROTOCOL_INFO, true, false,
                          WSAPROTOCOL_INFO>
    socket_option_protocol_info;
#endif
    typedef socket_option<SOL_SOCKET, SO_RCVBUF, true, true, int>
      socket_option_rcvbuf;
    typedef socket_option<SOL_SOCKET, SO_REUSEADDR, true, true, bool>
      socket_option_reuseaddr;
    typedef socket_option<SOL_SOCKET, SO_SNDBUF, true, true, int>
      socket_option_sndbuf;
    typedef socket_option<SOL_SOCKET, SO_TYPE, true, false, int>
      socket_option_type;
    // PVD_CONFIG

  }// namespace
}// namespace

#endif
