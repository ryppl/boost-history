//                 -- basic_client.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_CLIENT_HPP_INCLUDED__
#define CGI_BASIC_CLIENT_HPP_INCLUDED__

#include <boost/shared_ptr.hpp>

#include "boost/cgi/map.hpp"
#include "boost/cgi/role_type.hpp"
#include "boost/cgi/status_type.hpp"
#include "boost/cgi/http/status_code.hpp"
#include "boost/cgi/connections/tcp_socket.hpp"

namespace cgi {

  /// A client
  /**
   * A client is for two things:
   * 1. To hold a full-duplex connection (or separate input and output
   *    connections).
   * 2. To hold any protocol-specific data about the request. For now,
   *    this means the internal 'request number' associated by FastCGI
   *    with each request (ie. so incoming/outgoing packets can be wrapped
   *    with data noting what request it relates to).
   * 3. Buffering. Not sure about how far this should go yet, but probably
   *    no further than minimal buffering.
   * 4. Share a connection. Since a multiplexing connection is shared between
   *    multiple clients, the client should be responsible for taking possesion
   *    of the connection for a period of time (so it can write a complete
   *    packet). This idea could be taken quite far into genericity by making
   *    clients aware of how busy the connection is and size its output packets
   *    accordingly... But I'm not doing that.
   */
  template<typename Connection>
  class basic_client
  {
  public:
    typedef cgi::map                          map_type;
    typedef Protocol                          protocol_type;
    typedef Connection                        connection_type;
    typedef typename connection_type::pointer connection_ptr;

    basic_client(cgi::io_service& ios)
      : io_service_(ios)
    {
    }

    io_service& io_service() { return io_service_; }

    /// Associate a connection with this client
    /**
     * Note: the connection must have been created using the new operator
     */
    bool set_connection(connection_type* conn)
    {
      // make sure there isn't already a connection associated with the client
      if (!connection_) return false;
      connection_.reset(conn);
      return true;
    }

    /// Get a shared_ptr of the connection associated with the client.
    connection_ptr& connection() { return connection_; }

    /// Write some data to the client.
    template<typename ConstBufferSequence>
    std::size_t write_some(const ConstBufferSequence& buf
                          , boost::system::error_code& ec)
    {
      return connection_->write_some(buf, ec);
    }

    /// Read some data from the client.
    template<typename MutableBufferSequence>
    std::size_t read_some(const MutableBufferSequence& buf
                         , boost::system::error_code& ec)
    {
      return connection_->read_some(buf, ec);
    }

    /// Asynchronously write some data to the client.
    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(const ConstBufferSequence& buf, Handler handler)
    {
      connection_->async_write_some(buf, handler);
    }

    /// Asynchronously read some data from the client.
    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(const MutableBufferSequence& buf, Handler handler)
    {
      connection_->async_read_some(buf, handler);
    }
  private:
    io_service&                           io_service_;
    connection_ptr                        connection_;
  };

} // namespace cgi

#endif // CGI_BASIC_CLIENT_HPP_INCLUDED__
