// Copyright (C) 2002 Hugo Duncan

// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  Hugo Duncan makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

// based on code posted by Dylan ???  to
// http://lists.boost.org/MailArchives/boost/msg21797.php

#ifdef _MSC_VER
#pragma once
#endif

/// include guard
#ifndef SOCKET_STREAMBUF_HPP
#define SOCKET_STREAMBUF_HPP 1

#include "boost/socket/socket_base.hpp"
#include "boost/socket/socket_errors.hpp"

#include <string>

namespace boost
{
  namespace socket
  {

    template <typename Element,
              typename Traits = std::char_traits<Element>,
              typename ErrorPolicy=default_error_policy
              >
    class basic_socket_streambuf
      : public std::basic_streambuf<Element, Traits>
    {
    public:
      typedef ErrorPolicy error_policy;
      typedef Element char_type;
      typedef Traits traits_type;
      typedef typename Traits::int_type int_type;

      explicit basic_socket_streambuf(socket_base<error_policy> & sock,
                                      std::size_t bufsize=4096)
          : socket_(sock)
      {
        Element* buf;
        buf = new Element[bufsize];
        setg(buf, buf, buf);
        egbuf_ = buf + bufsize;
        buf = new Element[bufsize];
        setp(buf, buf + bufsize);
      }

      virtual ~basic_socket_streambuf()
      {
        delete [] eback();
        delete [] pbase();
      }

      unsigned long avail() const
      {
        if (gptr() < egptr())
          return egptr() - gptr();
        u_long ret = 0;
        ioctlsocket(socket_.socket(), FIONREAD, &ret);
        return ret;
      }

    protected:

      virtual int_type overflow(int_type c = Traits::eof())
      {
        if (Traits::eq_int_type(Traits::eof(), c))
          return Traits::not_eof(c);
        if (pptr() < epptr())
        {
          *pptr() = Traits::to_char_type(c);
        }
        else
          sync();
        return c;
      }

      virtual int_type pbackfail(int_type c = Traits::eof())
      {
        if (eback() < gptr())
        {
          gbump(-1);
          if (!Traits::eq_int_type(Traits::eof(), c))
            *gptr() = c;
          return Traits::not_eof(c);
        }
        return Traits::eof();
      }

      virtual int_type underflow()
      {
        if (gptr() < egptr())
          return Traits::to_int_type(*gptr());
        if (egbuf() - gptr() < gptr() - eback())
          setg(eback(), eback(), eback());
        size_type r=socket_.recv((void*)gptr(), egbuf() - gptr());
        if (r == 0)
          return Traits::eof();
        else if (r == socket_error)
        {
          int err=error_policy::handle_error();
          if (err!=WouldBlock)
            throw socket_exception("error in stream",err);
        }
        setg(eback(), gptr(), gptr() + r);
        return *gptr();
      }

      virtual int sync()
      {
        int len = pptr() - pbase();
        if (socket_.send((void*)pbase(), len) != len)
          return -1;
        setp(pbase(), epptr());
        return 0;
      }

    private:
      inline Element* egbuf()
      {
        return egbuf_;
      }

      Element* egbuf_;
      socket_base<error_policy> socket_;
    };

  }// namespace
}// namespace

#endif
