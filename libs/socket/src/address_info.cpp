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

#if defined(USES_WINSOCK2)
#define HAVE_GETADDRINFO
#endif



namespace boost
{
  namespace socket
  {

#if !defined(HAVE_GETADDRINFO)
    namespace detail
    {

      struct addrinfo {
        int     ai_flags;
        int     ai_family;
        int     ai_socktype;
        int     ai_protocol;
        size_t  ai_addrlen;
        char   *ai_canonname;
        struct sockaddr  *ai_addr;
        struct addrinfo  *ai_next;
      };


      void freeaddrinfo(addrinfo* info)
      {
        delete info;
      }

      int getaddrinfo(
        const char* name,
        const char* service,
        addrinfo* hint,
        addrinfo** result )
      {
        *result=0;
        return 0;
      }
    }
#define ADDRINFO_NS detail

    inline detail::addrinfo*&
    cast_addrinfo(boost::socket::addrinfo*& addr)
    {
      return (detail::addrinfo*&)(addr);
    }

    inline detail::addrinfo const*
    cast_addrinfo(boost::socket::addrinfo const* addr)
    {
      return (detail::addrinfo const*)(addr);
    }

#else
#define ADDRINFO_NS

    inline ::addrinfo*&
    cast_addrinfo(boost::socket::addrinfo*& addr)
    {
      return (::addrinfo*&)(addr);
    }

    inline ::addrinfo const*
    cast_addrinfo(const boost::socket::addrinfo* addr)
    {
      return (::addrinfo const*)(addr);
    }
#endif


    any_protocol address_info::protocol() const
    {
      return any_protocol(cast_addrinfo(m_addrinfo)->ai_socktype,
                          cast_addrinfo(m_addrinfo)->ai_protocol,
                          cast_addrinfo(m_addrinfo)->ai_family);
    }

    any_address address_info::address() const
    {
      return any_address(cast_addrinfo(m_addrinfo)->ai_family,
                         cast_addrinfo(m_addrinfo)->ai_addr,
                         cast_addrinfo(m_addrinfo)->ai_addrlen);
    }

    const int address_info::flags() const
    {
      return cast_addrinfo(m_addrinfo)->ai_flags;
    }

    addrinfo const* address_info::next() const
    {
      return (addrinfo const*)cast_addrinfo(m_addrinfo)->ai_next;
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
      ADDRINFO_NS::freeaddrinfo(cast_addrinfo(m_addrinfo));
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
      ADDRINFO_NS::addrinfo hint;

      memset(&hint, 0, sizeof(hint));
      hint.ai_flags = flags;
      hint.ai_family = family;
      hint.ai_socktype = socktype;
      hint.ai_protocol = protocol;

      result = ADDRINFO_NS::getaddrinfo(
        name, service, &hint, &cast_addrinfo(m_addrinfo));
    }

  }// namespace
}// namespace

#ifdef _MSC_VER
#pragma warning (pop)
#endif
