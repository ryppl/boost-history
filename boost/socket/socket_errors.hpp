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
#ifndef SOCKET_ERRORS_HPP
#define SOCKET_ERRORS_HPP 1

#include "boost/socket/config.hpp"

#ifdef USES_WINSOCK2
#include <winsock2.h>
#endif

#include <exception>

namespace boost
{
  namespace socket
  {

    struct socket_exception : public std::exception
    {

      virtual const char *what( ) const
#ifdef __GNUC__
        throw()
#endif
      {
        return "boost::socket::socket_exception";
      }
    };

    enum SocketError
    {
      Success=0,
#if defined(USES_WINSOCK2)
      WouldBlock=WSAEWOULDBLOCK
#else
      WouldBlock=EAGAIN
#endif
    };

#if defined(USES_WINSOCK2)
    SocketError check_error()
    {
      int error=WSAGetLastError();
      switch (error)
      {
        case WSAEACCES : throw "Permission denied";
        case WSAEADDRINUSE : throw "Address already in use";
        case WSAEADDRNOTAVAIL : throw "Cannot assign requested address";
        case WSAEAFNOSUPPORT :
          throw "Address family not supported by protocol family";
        case WSAEALREADY : throw "Operation already in progress";
        case WSAECONNABORTED : throw "Software caused connection abort";
        case WSAECONNREFUSED : throw "Connection refused";
        case WSAECONNRESET : throw "Connection reset by peer";
        case WSAEDESTADDRREQ : throw "Destination address required";
        case WSAEFAULT : throw "Bad address";
        case WSAEHOSTDOWN : throw "Host is down";
        case WSAEHOSTUNREACH : throw "No route to host";
        case WSAEINPROGRESS : throw "Operation now in progress";
        case WSAEINTR :throw "Interrupted function call";
        case WSAEINVAL : throw "Invalid argument";
        case WSAEISCONN : throw "Socket is already connected";
        case WSAEMFILE : throw "Too many open files";
        case WSAEMSGSIZE : throw "Message too long";
        case WSAENETDOWN : throw "Network is down";
        case WSAENETRESET : throw "Network dropped connection on reset";
        case WSAENETUNREACH : throw "Network is unreachable";
        case WSAENOBUFS : throw "No buffer space available.";
        case WSAENOPROTOOPT : throw "Bad protocol option";
        case WSAENOTCONN : throw "Socket is not connected";
        case WSAENOTSOCK : throw "Socket operation on nonsocket.";
        case WSAEOPNOTSUPP : throw "Operation not supported.";
        case WSAEPFNOSUPPORT : throw "Protocol family not supported.";
        case WSAEPROCLIM : throw "Too many processes.";
        case WSAEPROTONOSUPPORT : throw "Protocol not supported.";
        case WSAEPROTOTYPE : throw "Protocol wrong type for socket.";
        case WSAESHUTDOWN : throw "Cannot send after socket shutdown.";
        case WSAESOCKTNOSUPPORT : throw "Socket type not supported.";
        case WSAETIMEDOUT : throw "Connection timed out.";
        case WSATYPE_NOT_FOUND : throw "Class type not found.";
        case WSAEWOULDBLOCK :
          return WouldBlock;
        case WSAHOST_NOT_FOUND : throw "Host not found.";
        case WSA_INVALID_HANDLE : throw "Specified event object handle is invalid.";
        case WSA_INVALID_PARAMETER : throw "One or more parameters are invalid.";
//           case WSAINVALIDPROCTABLE : throw "Invalid procedure table from service provider.";
//           case WSAINVALIDPROVIDER : throw "Invalid service provider version number.";
        case WSA_IO_INCOMPLETE : throw "Overlapped I/O event object not in signaled state.";
        case WSA_IO_PENDING : throw "Overlapped operations will complete later.";
        case WSA_NOT_ENOUGH_MEMORY : throw "Insufficient memory available.";
        case WSANOTINITIALISED : throw "Successful WSAStartup not yet performed.";
        case WSANO_DATA : throw "Valid name, no data record of requested type.";
        case WSANO_RECOVERY : throw "This is a nonrecoverable error.";
//           case WSAPROVIDERFAILEDINIT : throw "Unable to initialize a service provider.";
        case WSASYSCALLFAILURE : throw "System call failure.";
        case WSASYSNOTREADY : throw "Network subsystem is unavailable.";
        case WSATRY_AGAIN : throw "Nonauthoritative host not found.";
        case WSAVERNOTSUPPORTED : throw "Winsock.dll version out of range.";
        case WSAEDISCON : throw "Graceful shutdown in progress.";
        case WSA_OPERATION_ABORTED : throw "Overlapped operation aborted.";
        default:
          throw "unknown error";
      }
    }

#else

    SocketError check_error()
    {
      switch (errno)
      {
        case EAGAIN : return WouldBlock ;
        case EBADF : throw "not a valid descriptor.";
        case EOPNOTSUPP :throw "Operation not supported on transport endpoint";
        case EPFNOSUPPORT : throw "Protocol family not supported";
        case ECONNRESET : throw "Connection reset by peer";
        case ENOBUFS : throw "No buffer space available";
        case EAFNOSUPPORT : throw "Address family not supported by protocol family";
        case EPROTOTYPE : throw "Protocol wrong type for socket";
        case ENOTSOCK : throw "Socket operation on non-socket";
        case ENOPROTOOPT : throw "Protocol not available";
        case ESHUTDOWN : throw "Can't send after socket shutdown";
        case ECONNREFUSED : throw "Connection refused";
        case EADDRINUSE :  throw "Address already in use";
        case ECONNABORTED : throw "Connection aborted";
        case ENETUNREACH :  throw "Network is unreachable";
        case ENETDOWN :  throw "Network interface is not configured";
        case ETIMEDOUT :  throw "Connection timed out";
        case EHOSTDOWN :  throw "Host is down";
        case EHOSTUNREACH : throw "Host is unreachable";
        case EINPROGRESS :  throw "Connection already in progress";
        case EALREADY :  throw "Socket already connected";
        case EDESTADDRREQ : throw "Destination address required";
        case EMSGSIZE :  throw "Message too long";
        case EPROTONOSUPPORT : throw "Unknown protocol";
        case ESOCKTNOSUPPORT : throw "Socket type not supported";
        case EADDRNOTAVAIL : throw "Address not available";
        case ENETRESET : throw "net reset";
        case EISCONN :  throw "Socket is already connected";
        case ENOTCONN :  throw "Socket is not connected";

        default:
          throw "unknown error";
      }
    }

#endif

  }// namespace
}// namespace

#endif
