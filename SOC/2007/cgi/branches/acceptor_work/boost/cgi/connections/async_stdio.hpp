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
#include <boost/asio/posix/stream_descriptor.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/common/tags.hpp"
#include "boost/cgi/import/io_service.hpp"

#if ! defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
#    include "boost/cgi/connections/stdio.hpp"
#endif // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

namespace cgi {
 namespace common {

#if defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

  template<>
  class basic_connection<tags::async_stdio>
  {
  public:
    typedef basic_connection<tags::async_stdio>    type;
    typedef boost::shared_ptr<type>                pointer;
    typedef boost::asio::posix::stream_descriptor  next_layer_type;

    basic_connection(common::io_service& ios)
      : in_(ios, 0)
      , out_(ios, 1)
    {
    }

    bool is_open() const
    {
      return in_.is_open() && out_.is_open();
    }
    
    void close()
    {
      in_.close();
      out_.close();
    }

    static pointer create(::cgi::common::io_service& ios)
    {
      return pointer(new basic_connection<tags::async_stdio>(ios));
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(const MutableBufferSequence& buf, boost::system::error_code& ec)
    {
      return in_.read_some(buf, ec);
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(const MutableBufferSequence& buf, Handler handler)
    {
      in_.async_read_some(buf, handler);
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(const ConstBufferSequence& buf, boost::system::error_code& ec)
    {
      return out_.write_some(buf, ec);
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(const ConstBufferSequence& buf, Handler handler)
    {
      out_.async_write_some(buf, handler);
    }

    boost::asio::posix::stream_descriptor in_;
    boost::asio::posix::stream_descriptor out_;
  };

#else

  /// Simple, pseudo-asynchronous access to stdio.
  /**
   * This will only appear asynchronous if the io_service is being run
   * in multiple threads. This doesn't spawn any hidden threads.
   */
  template<>
  class basic_connection<tags::async_stdio>
    : public basic_connection<tags::stdio>
    , public boost::enable_shared_from_this<basic_connection<tags::async_stdio> >
  {
  public:
    typedef basic_connection<tags::async_stdio> type;
    typedef boost::shared_ptr<type>             pointer;

    basic_connection(common::io_service& ios)
      : basic_connection<tags::stdio>()
      , io_service_(ios)
    {
    }

    bool is_open() const
    {
      return is_open_;
    }

    void close()
    {
      is_open_ = false;
    }

    static pointer create(::cgi::common::io_service& ios)
    {
      return pointer(new basic_connection<tags::async_stdio>(ios));
    }

    template<typename ConnectionPtr, typename MutableBufferSequence
            , typename Handler>
    class read_handler
    {
    public:
      read_handler(ConnectionPtr cptr, MutableBufferSequence& buf
                  , Handler handler)
        : cptr_(cptr)
        , buf_(buf)
        , handler_(handler)
      {
      }

      void operator()()
      {
        boost::system::error_code ec;
        std::size_t bytes_read = cptr_->read_some(buf_, ec);
        handler_(ec, bytes_read);
      }

    private:
      ConnectionPtr cptr_;
      MutableBufferSequence& buf_;
      Handler handler_;
    };

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(const MutableBufferSequence& buf, Handler handler)
    {
      io_service_.post(read_handler<pointer, MutableBufferSequence, Handler>
                         (shared_from_this(), buf, handler));
    }

    template<typename ConnectionPtr, typename ConstBufferSequence
            , typename Handler>
    class write_handler
    {
    public:
      write_handler(ConnectionPtr cptr, ConstBufferSequence& buf
                   , Handler handler)
        : cptr_(cptr)
        , buf_(buf)
        , handler_(handler)
      {
      }

      void operator()()
      {
        boost::system::error_code ec;
        std::size_t bytes_written = cptr_->write_some(buf_, ec);
        handler_(ec, bytes_written);
      }

    private:
      ConnectionPtr cptr_;
      ConstBufferSequence& buf_;
      Handler handler_;
    };

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(const ConstBufferSequence& buf, Handler handler)
    {
      io_service_.post(write_handler<pointer, ConstBufferSequence, Handler>
                         (shared_from_this(), buf, handler));
    }

  private:
    ::cgi::common::io_service& io_service_;
  };

#endif // defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

  typedef basic_connection<tags::async_stdio> async_stdio_connection;


  //  template<typename ProtocolService = detail::async_cgi_service>
  //struct async_stdio_connection
  //{
  //  typedef basic_connection<tags::async_cgi, ProtocolService>    type;
  //};

 } // namespace common
} // namespace cgi

#endif // CGI_ASYNC_STDIO_HPP_INCLUDED__
