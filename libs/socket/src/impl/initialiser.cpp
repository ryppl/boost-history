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

#include "boost/socket/impl/initialiser.hpp"
#include "boost/socket/socket_exception.hpp"
#include "boost/socket/config.hpp"

#ifdef _MSC_VER
#pragma warning (push, 4)
#pragma warning (disable: 4786 4305)
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  #include <Winsock2.h>
#endif

namespace boost
{
  namespace socket
  {
    namespace impl
    {
      bool initialiser::m_initialised=false;

      const initialiser& initialiser::uses_platform()
      {
        static initialiser init;
        if (!m_initialised)
          initialise();
        return init;
      }

      initialiser::~initialiser()
      {
        if (m_initialised)
          finalise();
      }

      void initialiser::initialise()
      {
#ifdef _WIN32
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

        wVersionRequested = ((MAKEWORD(2, 2)));

        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0)
        {
          throw socket_exception("Failed to initialise system",
                                 system_error);
        }
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
          WSACleanup();
          return;
        }

        m_initialised=true;
#endif
      }

      void initialiser::finalise()
      {
#ifdef _WIN32
        WSACleanup();
#endif
      }

    }// namespace
  }// namespace
}// namespace

#ifdef _MSC_VER
#pragma warning (pop)
#endif
