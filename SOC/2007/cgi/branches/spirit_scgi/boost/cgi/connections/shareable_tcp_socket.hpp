//       -- connections/shareable_tcp_socket.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CONNECTIONS_SHAREABLE_TCP_SOCKET_HPP_INCLUDED__
#define CGI_CONNECTIONS_SHAREABLE_TCP_SOCKET_HPP_INCLUDED__

#include <boost/shared_ptr.hpp>

#include "boost/cgi/tags.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/connection_base.hpp"
#include "boost/cgi/basic_connection.hpp"
#include "boost/cgi/detail/push_options.hpp"

namespace cgi {

  template<>
  class basic_connection<tags::shareable_tcp_socket>
    : public connection_base
  {
  public:
    typedef boost::shared_ptr<
      basic_connection<tags::shareable_tcp_socket> >  pointer;
    typedef boost::mutex                              mutex_type;

    // A wrapper to provide condition_type::pointer
    struct condition_type : public boost::condition
    { typedef boost::shared_ptr<boost::condition> pointer; };

    basic_connection(io_service& ios)
      : sock_(ios)
      , mutex_()
      , condition_()
    {
    }

    static pointer create(io_service& ios)
    {
      return static_cast<pointer>(
        new basic_connection<tags::shareable_tcp_socket>(ios));
    }      

    template<typename MutableBufferSequence>
    std::size_t read_some(MutableBufferSequence& buf)
    {
      return sock_.read_some(buf);
    }

    template<typename MutableBufferSequence>
    std::size_t read_some(MutableBufferSequence& buf
                         , boost::system::error_code& ec)
    {
      return sock_.read_some(buf, ec);
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(MutableBufferSequence& buf, Handler handler)
    {
      sock_.async_read_some(buf, handler);
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence& buf)
    {
      return sock_.write_some(buf);
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return sock_.write_some(buf, ec);
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(ConstBufferSequence& buf, Handler handler)
    {
      sock_.async_write_some(buf, handler);
    }

    void close()
    {
      sock_.close();
    }

    mutex_type& mutex()        { return mutex_;     }
    condtion_type& condition() { return condition_; }
  private:
    
    boost::asio::ip::tcp::socket sock_;
    mutex_type mutex_;
    condition_type condition_;
  };

  // probably deletable typedef (leaving it here to keep an open mind)
  typedef basic_connection<tags::shareable_tcp_socket> shareable_tcp_connection;

  namespace connection {
    typedef basic_connection<tags::shareable_tcp_socket> shareable_tcp;
  } // namespace connection

} // namespace cgi

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_CONNECTIONS_SHAREABLE_TCP_SOCKET_HPP_INCLUDED__
