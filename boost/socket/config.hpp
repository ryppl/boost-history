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
#ifndef BOOST_SOCKETS_CONFIG_HPP
#define BOOST_SOCKETS_CONFIG_HPP 1

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
 #include <Winsock2.h>
 #define USES_WINSOCK2 1

#elif defined(__CYGWIN__)

#else

#endif

namespace boost
{
  namespace socket
  {

#if defined(USES_WINSOCK2)
 typedef SOCKET socket_type;
 typedef int size_type;
 enum { socket_error = SOCKET_ERROR,
        invalid_socket = INVALID_SOCKET
      };

#elif defined(__CYGWIN__)
 typedef int socket_type;
 typedef int size_type;
 enum {
   socket_error = -1,
   invalid_socket = -1
 };

#else
 typedef int socket_type;
 typedef socklen_t size_type;
 enum {
   socket_error = -1,
   invalid_socket = -1
 };
#endif


  } //namespace
} //namespace

#endif
