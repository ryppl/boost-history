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
#else
#include <sys/errno.h>
#endif

#include <exception>
#include <string>

namespace boost
{
  namespace socket
  {
    enum error_type { system_error };

    // At the moment, this mirrors the approch taken
    // by the filesystem library.
    // this should follow whatever idiom is decided upon
    // for exceptions in boost in general
    class socket_exception : public std::exception
    {
    public:
      explicit socket_exception( std::string const& msg )
          : m_msg(msg), m_err(0)
      {}

      explicit socket_exception( std::string const& msg, int value)
          : m_msg(msg), m_err(value)
      {}

      ~socket_exception() throw () {}

      virtual const char *what( ) const throw()
      {
        return "boost::socket::socket_exception";
      }

      int error() const { return m_err; }
      const std::string& message() const { return m_msg; }

    private:
      std::string m_msg;
      int         m_err;
    };

    enum SocketError
    {
      Success=0,
      WouldBlock=-1,
      WouldBlockRead=-2,
      WouldBlockWrite=-3
    };

#if defined(USES_WINSOCK2)
    struct default_error_policy
    {

      static int handle_error()
      {
        int error=WSAGetLastError();
        switch (error)
        {
          case WSAEACCES :
            throw socket_exception("Permission denied",error);
          case WSAEADDRINUSE :
            throw socket_exception("Address already in use",error);
          case WSAEADDRNOTAVAIL :
            throw socket_exception("Cannot assign requested address",error);
          case WSAEAFNOSUPPORT :
            throw socket_exception("Address family not supported by protocol family",error);
          case WSAEALREADY :
            throw socket_exception("Operation already in progress",error);
          case WSAECONNABORTED :
            throw socket_exception("Software caused connection abort",error);
          case WSAECONNREFUSED :
            throw socket_exception("Connection refused",error);
          case WSAECONNRESET :
            throw socket_exception("Connection reset by peer",error);
          case WSAEDESTADDRREQ :
            throw socket_exception("Destination address required",error);
          case WSAEFAULT :
            throw socket_exception("Bad address",error);
          case WSAEHOSTDOWN :
            throw socket_exception("Host is down",error);
          case WSAEHOSTUNREACH :
            throw socket_exception("No route to host",error);
          case WSAEINPROGRESS :
            throw socket_exception("Operation now in progress",error);
          case WSAEINTR :
            throw socket_exception("Interrupted function call",error);
          case WSAEINVAL :
            throw socket_exception("Invalid argument",error);
          case WSAEISCONN : throw socket_exception("Socket is already connected",error);
          case WSAEMFILE :
            throw socket_exception("Too many open files",error);
          case WSAEMSGSIZE :
            throw socket_exception("Message too long",error);
          case WSAENETDOWN :
            throw socket_exception("Network is down",error);
          case WSAENETRESET :
            throw socket_exception("Network dropped connection on reset",error);
          case WSAENETUNREACH :
            throw socket_exception("Network is unreachable",error);
          case WSAENOBUFS :
            throw socket_exception("No buffer space available.",error);
          case WSAENOPROTOOPT :
            throw socket_exception("Bad protocol option",error);
          case WSAENOTCONN :
            throw socket_exception("Socket is not connected",error);
          case WSAENOTSOCK :
            throw socket_exception("Socket operation on nonsocket.",error);
          case WSAEOPNOTSUPP :
            throw socket_exception("Operation not supported.",error);
          case WSAEPFNOSUPPORT :
            throw socket_exception("Protocol family not supported.",error);
          case WSAEPROCLIM :
            throw socket_exception("Too many processes.",error);
          case WSAEPROTONOSUPPORT :
            throw socket_exception("Protocol not supported.",error);
          case WSAEPROTOTYPE :
            throw socket_exception("Protocol wrong type for socket.",error);
          case WSAESHUTDOWN :
            throw socket_exception("Cannot send after socket shutdown.",error);
          case WSAESOCKTNOSUPPORT :
            throw socket_exception("Socket type not supported.",error);
          case WSAETIMEDOUT :
            throw socket_exception("Connection timed out.",error);
          case WSATYPE_NOT_FOUND :
            throw socket_exception("Class type not found.",error);
          case WSAEWOULDBLOCK :
            return WouldBlock;
          case WSAHOST_NOT_FOUND :
            throw socket_exception("Host not found.",error);
          case WSA_INVALID_HANDLE :
            throw socket_exception("Specified event object handle is invalid.",error);
          case WSA_INVALID_PARAMETER :
            throw socket_exception("One or more parameters are invalid.",error);
          case WSA_IO_INCOMPLETE :
            throw "Overlapped I/O event object not in signaled state.";
          case WSA_IO_PENDING :
            throw socket_exception("Overlapped operations will complete later.",error);
          case WSA_NOT_ENOUGH_MEMORY :
            throw socket_exception("Insufficient memory available.",error);
          case WSANOTINITIALISED :
            throw socket_exception("Successful WSAStartup not yet performed.",error);
          case WSANO_DATA :
            throw socket_exception("Valid name, no data record of requested type.",error);
          case WSANO_RECOVERY :
            throw socket_exception("This is a nonrecoverable error.",error);
          case WSASYSCALLFAILURE :
            throw socket_exception("System call failure.",error);
          case WSASYSNOTREADY :
            throw socket_exception("Network subsystem is unavailable.",error);
          case WSATRY_AGAIN :
            throw socket_exception("Nonauthoritative host not found.",error);
          case WSAVERNOTSUPPORTED :
            throw socket_exception("Winsock.dll version out of range.",error);
          case WSAEDISCON :
            throw socket_exception("Graceful shutdown in progress.",error);
          case WSA_OPERATION_ABORTED :
            throw socket_exception("Overlapped operation aborted.",error);
          default:
            throw socket_exception("unknown error",error);
        }
      }
    };


#else

    struct default_error_policy
    {

      static SocketError handle_error()
      {
        switch (errno)
        {
          case EAGAIN :
            return WouldBlock ;
          case EBADF :
            throw socket_exception("not a valid descriptor.",errno);
          case EOPNOTSUPP :
            throw socket_exception("Operation not supported on transport endpoint",errno);
          case EPFNOSUPPORT :
            throw socket_exception("Protocol family not supported",errno);
          case ECONNRESET :
            throw socket_exception("Connection reset by peer",errno);
          case ENOBUFS :
            throw socket_exception("No buffer space available",errno);
          case EAFNOSUPPORT :
            throw socket_exception("Address family not supported by protocol family",errno);
          case EPROTOTYPE :
            throw socket_exception("Protocol wrong type for socket",errno);
          case ENOTSOCK :
            throw "Socket operation on non-socket";
          case ENOPROTOOPT :
            throw socket_exception("Protocol not available",errno);
          case ESHUTDOWN :
            throw socket_exception("Can't send after socket shutdown",errno);
          case ECONNREFUSED :
            throw socket_exception("Connection refused",errno);
          case EADDRINUSE :
            throw socket_exception("Address already in use",errno);
          case ECONNABORTED :
            throw socket_exception("Connection aborted",errno);
          case ENETUNREACH :
            throw socket_exception("Network is unreachable",errno);
          case ENETDOWN :
            throw socket_exception("Network interface is not configured",errno);
          case ETIMEDOUT :
            throw socket_exception("Connection timed out",errno);
          case EHOSTDOWN :
            throw socket_exception("Host is down",errno);
          case EHOSTUNREACH :
            throw socket_exception("Host is unreachable",errno);
          case EINPROGRESS :
            throw socket_exception("Connection already in progress",errno);
          case EALREADY :
            throw socket_exception("Socket already connected",errno);
          case EDESTADDRREQ :
            throw socket_exception("Destination address required",errno);
          case EMSGSIZE :
            throw socket_exception("Message too long",errno);
          case EPROTONOSUPPORT :
            throw socket_exception("Unknown protocol",errno);
          case ESOCKTNOSUPPORT :
            throw socket_exception("Socket type not supported",errno);
          case EADDRNOTAVAIL :
            throw socket_exception("Address not available",errno);
          case ENETRESET :
            throw socket_exception("net reset",errno);
          case EISCONN :
            throw socket_exception("Socket is already connected",errno);
          case ENOTCONN :
            throw socket_exception("Socket is not connected",errno);
          default:
            throw socket_exception("unknown error",errno);
        }
      }
    };
#endif

  }// namespace
}// namespace

#endif

