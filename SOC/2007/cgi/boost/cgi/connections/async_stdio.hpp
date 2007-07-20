//                  -- async_stdio.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ASYNC_STDIO_HPP_INCLUDED__
#define CGI_ASYNC_STDIO_HPP_INCLUDED__

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "stdio.hpp"
#include "../io_service.hpp"
#include "../tags.hpp"

namespace cgi {

  // Asynchronous access to stdio
  template<>
  class basic_connection<tags::async_stdio>
    : public basic_connection<tags::stdio>
    , public boost::enable_shared_from_this<basic_connection<tags::async_stdio> >
  {
  public:
    typedef basic_connection<tags::async_stdio> type;
    typedef boost::shared_ptr<type>             pointer;

    basic_connection(io_service& ios)
      : basic_connection<tags::stdio>()
      , io_service_(ios)
    {
    }

    template<typename ConnectionPtr, typename MutableBufferSequence
            , typename Handler>
    class read_handler
    {
    public:
      read_handler(ConnectionPtr cptr, MutableBufferSequence& buf
                  , boost::system::error_code& ec, Handler handler)
        : cptr_(cptr)
        , buf_(buf)
        , ec_(ec)
        , handler_(handler)
      {
      }

      void operator()()
      {
        std::size_t bytes_read = cptr_->read_some(buf_, ec_);
        handler_(ec_, bytes_read);
      }

    private:
      ConnectionPtr cptr_;
      MutableBufferSequence& buf_;
      boost::system::error_code& ec_;
      Handler handler_;
    };

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(MutableBufferSequence buf, boost::system::error_code& ec
                        , Handler handler)
    {
      io_service_.post(read_handler<pointer, MutableBufferSequence, Handler>
                         (shared_from_this(), buf, ec, handler));
    }

    template<typename ConnectionPtr, typename ConstBufferSequence
            , typename Handler>
    class write_handler
    {
    public:
      write_handler(ConnectionPtr cptr, ConstBufferSequence& buf
                   , boost::system::error_code& ec, Handler handler)
        : cptr_(cptr)
        , buf_(buf)
        , ec_(ec)
        , handler_(handler)
      {
      }

      void operator()()
      {
        std::size_t bytes_written = cptr_->write_some(buf_, ec_);
        handler_(ec_, bytes_written);
      }

    private:
      ConnectionPtr cptr_;
      ConstBufferSequence& buf_;
      boost::system::error_code& ec_;
      Handler handler_;
    };

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(ConstBufferSequence& buf, boost::system::error_code& ec
                         , Handler handler)
    {
      io_service_.post(write_handler<pointer, ConstBufferSequence, Handler>
                         (shared_from_this(), buf, ec, handler));
    }

  private:
    io_service& io_service_;
  };

  typedef basic_connection<tags::async_stdio> async_stdio_connection;


  //  template<typename ProtocolService = detail::async_cgi_service>
  //struct async_stdio_connection
  //{
  //  typedef basic_connection<tags::async_cgi, ProtocolService>    type;
  //};

} // namespace cgi

#endif // CGI_ASYNC_STDIO_HPP_INCLUDED__
