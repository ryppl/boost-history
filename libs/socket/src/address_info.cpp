// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifdef _MSC_VER
#pragma warning (disable: 4786 4305)
  // 4786 truncated debug symbolic name
  // 4305 truncation from const double to float
#endif

#if defined(__BORLANDC__)
#pragma hdrstop
#endif

#include "boost/socket/address_info.hpp"
#include "boost/socket/any_protocol.hpp"
#include "boost/socket/any_address.hpp"

#ifdef USES_WINSOCK2
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#endif

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

namespace boost
{
  namespace socket
  {

    any_protocol address_info::protocol() const
    {
      return any_protocol(m_addrinfo->ai_socktype,
                          m_addrinfo->ai_protocol,
                          m_addrinfo->ai_family);
    }

    any_address address_info::address() const
    {
      return any_address(m_addrinfo->ai_family,
                         m_addrinfo->ai_addr,
                         m_addrinfo->ai_addrlen);
    }

    const int address_info::flags() const
    {
      return m_addrinfo->ai_flags;
    }

    ::addrinfo const* address_info::next() const
    {
      return m_addrinfo->ai_next;
    }

    address_info_list::address_info_list(const char* name,
                                         const char* service,
                                         const int   flags,
                                         const int   family,
                                         const int   socktype,
                                         const int   protocol)
        : m_addrinfo(0)
    {
      get_addrinfo(name,service,flags,family,socktype,protocol);
    }

    address_info_list::~address_info_list()
    {
      ::freeaddrinfo(m_addrinfo);
    }

    void address_info_list::get_addrinfo(
      const char* name,
      const char* service,
      const int   flags,
      const int   family,
      const int   socktype,
      const int   protocol )
    {
      int result;
      ::addrinfo hint;

      memset(&hint, 0, sizeof(hint));
      hint.ai_flags = flags;
      hint.ai_family = family;
      hint.ai_socktype = socktype;
      hint.ai_protocol = protocol;

      result = ::getaddrinfo(name, service, &hint, &m_addrinfo);
//       if (result != 0)
//          throw socket_exception("getaddrinfo",result);
    }

  }// namespace
}// namespace

#ifdef _MSC_VER
#pragma warning (pop)
#endif
