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
#ifndef BOOST_SOCKET_CONFIG_HPP
#define BOOST_SOCKET_CONFIG_HPP 1

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
 #define USES_WINSOCK2 1
// #elif defined(__CYGWIN__)
// #else
#endif

namespace boost
{
  namespace socket
  {

    typedef unsigned int size_t;
    typedef short family_t;
    typedef unsigned short port_t;
    typedef int protocol_type_t;
    typedef int protocol_t;

    enum Direction
    {
      Receiving=0,
      Sending=1,
      Both=2
    };

    //! these are here until socket_set is made an implementation detail
#if defined(USES_WINSOCK2)
        typedef unsigned int socket_t;
#elif defined(__CYGWIN__)
        typedef int socket_t;
#else
        typedef int socket_t;
#endif


// #if defined(USES_WINSOCK2)
//  typedef unsigned long socket_type;
//  typedef int size_type;
//  typedef short family_type;
//  typedef unsigned short port_type;
//  enum { socket_error = -1,
//         invalid_socket = -1
//       };

// #elif defined(__CYGWIN__)
//  typedef int socket_type;
//  typedef int size_type;
//  typedef short family_type;
//  typedef unsigned short port_type;
//  enum {
//    socket_error = -1,
//    invalid_socket = -1
//  };

// #else
//  typedef int socket_type;
//  typedef socklen_t size_type;
//  typedef short family_type;
//  typedef unsigned short port_type;
//  enum {
//    socket_error = -1,
//    invalid_socket = -1
//  };
// #endif


  } //namespace
} //namespace

#endif
