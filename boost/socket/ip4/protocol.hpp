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
#ifndef BOOST_SOCKET_IP4_PROTOCOL_HPP
#define BOOST_SOCKET_IP4_PROTOCOL_HPP 1

#include "boost/socket/config.hpp"
#include "boost/socket/socket_option.hpp"

#if defined(USES_WINSOCK2)
#include <Ws2tcpip.h>
#else

#ifdef __CYGWIN__
#include "cygwin/in_systm.h"
#endif

#include <netinet/in.h>
#include <netinet/ip.h>
#endif

namespace boost
{
  namespace socket
  {
    namespace ip4
    {


//  typedef socket_option<SOL_SOCKET, SIO_GET_INTERFACE_LIST, true, false, ???>
//    socket_option_get_interface_list;

      //! IP options
      typedef socket_option<IPPROTO_IP, IP_OPTIONS, true, true, char *>
      socket_option_ip_options;
      typedef socket_option<IPPROTO_IP, IP_TOS, true, false, int>
      socket_option_ip_tos;
      typedef socket_option<IPPROTO_IP, IP_TTL, true, true, int>
      socket_option_ip_ttl;
#ifdef USES_WINSOCK2
      typedef socket_option<IPPROTO_IP, IP_HDRINCL, true, false, BOOL>
      socket_option_ip_hdrincl;
#endif

#ifdef USES_WINSOCK2
      typedef socket_option<IPPROTO_IP, IP_MULTICAST_IF, true, true, IN_ADDR*>
      socket_option_ip_multicast_if;
#elif !defined(__CYGWIN__)
      typedef socket_option<IPPROTO_IP, IP_MULTICAST_IF, true, true, ip_mreqn*>
      socket_option_ip_multicast_if;
#endif

      typedef socket_option<IPPROTO_IP, IP_MULTICAST_TTL, true, true, int>
      socket_option_ip_multicast_ttl;
      typedef socket_option<IPPROTO_IP, IP_MULTICAST_LOOP, true, true, bool_t>
      socket_option_ip_multicast_loop;
//     typedef socket_option<IPPROTO_IP, IP_ADD_MEMBERSHIP, true, true, IP_MREQ*>
//       socket_option_ip_add_membership;
//     typedef socket_option<IPPROTO_IP, IP_DROP_MEMBERSHIP, true, true, IP_MREQ*>
//       socket_option_ip_drop_membership;

      //! IP UDP options
#ifdef USES_WINSOCK2
      typedef socket_option<IPPROTO_UDP, UDP_NOCHECKSUM, true, true, bool_t>
      socket_option_udp_nochecksum;
#endif

      //! IP TCP options
#ifdef USES_WINSOCK2
      typedef socket_option<IPPROTO_TCP, TCP_EXPEDITED_1122, true, true,bool_t>
      socket_option_tcp_expedited_1122;
#endif

      class tcp_protocol
      {
      public:
        static int type()
        {
          return SOCK_STREAM;
        }

        static int protocol()
        {
          return IPPROTO_TCP;
        }

        static short family()
        {
          return AF_INET;
        }
      };

      class udp_protocol
      {
      public:
        static int type()
        {
          return SOCK_DGRAM;
        }

        static int protocol()
        {
          return IPPROTO_UDP;
        }

        static short family()
        {
          return PF_INET;
        }
      };


    }// namespace
  }// namespace
}// namespace

#endif
