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
#ifndef BOOST_SOCKET_IP4_ADDRESS_HPP
#define BOOST_SOCKET_IP4_ADDRESS_HPP 1

#include "boost/socket/config.hpp"

#ifdef USES_WINSOCK2

#include <Ws2tcpip.h>

#else

#ifdef __CYGWIN__
#include "cygwin/in_systm.h"
#endif

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <arpa/inet.h>

#endif

#include <cstring>

#include "boost/config.hpp"
#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::memset; using ::memcpy;  using ::memcmp; }
#endif

namespace boost
{
  namespace socket
  {
    namespace ip4
    {

      //! Address class for IP4
      class address
      {
      public:
        address()
        {
          std::memset(&address_,0, sizeof(address_));
          address_.sin_family = AF_INET;
        }

        short family() const
        {
          return address_.sin_family;
        }

        unsigned short port() const
        {
          return htons(address_.sin_port);
        }

        //! set the (host ordered) port number
        inline void port(unsigned short port)
        {
          address_.sin_port = htons(port);
        }

        void hostname(const char* hostname)
        {
          hostent *hp = gethostbyname(hostname);
          if (hp == 0)
            throw "hostname not found";
          std::memcpy((char*)&address_.sin_addr,
                      (char*)hp->h_addr, hp->h_length);
        }

        const char* hostname() const
        {
          hostent *hp = gethostbyaddr((const char*)&address_.sin_addr,
                                      sizeof(unsigned long),
                                      AF_INET );
          if (hp == 0)
            throw "hostname not found";
          return hp->h_name;
        }

        void ip(const char* ip_string)
        {
          unsigned long i = inet_addr(ip_string);
          if (i == INADDR_NONE)
            throw "ip not valid";
#ifdef USES_WINSOCK2
          address_.sin_addr.S_un.S_addr=i;
#else
          address_.sin_addr.s_addr=i;
#endif
        }

        const char* ip() const
        {
          const char* ret=inet_ntoa(address_.sin_addr);
          if (!ret)
            throw "ip address not representable";
          return ret;
        }

        // should return something that can be passed to other functions
        const sockaddr* socket_address() const
        {
          return reinterpret_cast<const ::sockaddr*>(&address_);
        }

        // should return something that can be passed to other functions
        /** This is to make sure we can passs the "correct" sockaddr structure,
            to eg. accept.
        */
        sockaddr* socket_address()
        {
          return reinterpret_cast< ::sockaddr*>(&address_);
        }

        static int size()
        {
          return sizeof(sockaddr_in);
        }

        bool operator < (const address& addr) const
        {
          const int cmp=std::memcmp(&address_.sin_addr,
                                    &addr.address_.sin_addr,
                                    sizeof(in_addr));
          return cmp<0
            || cmp==0 && address_.sin_port < addr.address_.sin_port;
        }

        bool operator == (const address& addr) const
        {
          const int cmp=std::memcmp(&address_.sin_addr,
                                    &addr.address_.sin_addr,
                                    sizeof(in_addr));
          return cmp==0 && address_.sin_port == addr.address_.sin_port;
        }

        bool operator != (const address& addr) const
        {
          const int cmp=std::memcmp(&address_.sin_addr,
                                    &addr.address_.sin_addr,
                                    sizeof(in_addr));
          return cmp!=0 || address_.sin_port != addr.address_.sin_port;
        }

      private:
        sockaddr_in address_;
      };
#if 0
      struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
      };
#endif

    }// namespace
  }// namespace
}// namespace

#endif
