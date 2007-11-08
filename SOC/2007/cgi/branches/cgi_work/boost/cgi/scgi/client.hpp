//                 -- scgi/client.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_SCGI_CLIENT_HPP_INCLUDED__
#define CGI_SCGI_CLIENT_HPP_INCLUDED__

#include "boost/cgi/map.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/basic_client.hpp"

namespace cgi {
 namespace scgi {

  /// A client that uses a TCP socket that owned by it.
  template<typename Connection>
  class basic_client<tcp_connection>
  {
  public:
    typedef ::cgi::io_service                 io_service_type;
    typedef cgi::map                          map_type;
    //typedef Protocol                          protocol_type;
    struct connection_type : Connection
    { typedef boost::shared_ptr<connection_type> pointer; }

    /// Construct
    basic_client()
    {
    }

    /// Construct
    basic_client(io_service_type& ios)
      //: io_service_(ios)
      //, connection_(new connection_type::pointer(ios))
    {
    }

    /// Destroy
    /** Closing the connection as early as possible is good for efficiency */
    ~basic_client()
    {
      if (connection_)
        connection_->close();
    }

    //io_service_type& io_service() { return io_service_; }

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

    /// Associate a connection with this client
    bool set_connection(connection_type::pointer& conn)
    {
      // make sure there isn't already a connection associated with the client
      if (!connection_) return false;
      connection_ = conn;
      return true;
    }

    /// Get a shared_ptr of the connection associated with the client.
    connection_type::pointer&
      connection() { return connection_; }

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
    connection_ptr   connection_;
  };

 } // namespace scgi
}// namespace cgi

#endif // CGI_SCGI_CLIENT_HPP_INCLUDED__
