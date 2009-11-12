//                 -- fcgi/client.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_FCGI_CLIENT_HPP_INCLUDED__
#define CGI_FCGI_CLIENT_HPP_INCLUDED__

#include <vector>
///////////////////////////////////////////////////////////
#include <boost/shared_ptr.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/asio/buffer.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/error.hpp"
#include "boost/cgi/common/map.hpp"
#include "boost/cgi/common/tags.hpp"
#include "boost/cgi/import/read.hpp"
#include "boost/cgi/basic_client.hpp"
#include "boost/cgi/import/buffer.hpp"
#include "boost/cgi/import/io_service.hpp"
#include "boost/cgi/fcgi/specification.hpp"
#include "boost/cgi/detail/throw_error.hpp"
#include "boost/cgi/fwd/basic_request_fwd.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"
#include "boost/cgi/connections/shareable_tcp_socket.hpp"

#undef min
#undef max
#include <algorithm>

BOOST_CGI_NAMESPACE_BEGIN
 namespace common {

  enum client_status
  {
    none_, // **FIXME** !
    constructed,
    params_read,
    stdin_read,
    end_request_sent,
    closed_, // **FIXME** !
    //aborted
  };

  /// A client that uses a TCP socket that owned by it.
  template<typename Protocol>
  class basic_client<connections::shareable_tcp, Protocol>
  {
  public:
    typedef ::BOOST_CGI_NAMESPACE::common::io_service   io_service_type;
    typedef ::BOOST_CGI_NAMESPACE::common::map          map_type;
    typedef Protocol                                    protocol_type;
    typedef connections::shareable_tcp                  connection_type;
    typedef typename connection_type::pointer           connection_ptr;
    typedef boost::array<
        unsigned char
      , fcgi::spec::header_length::value
    >                                         header_buffer_type;
    typedef boost::asio::mutable_buffers_1    mutable_buffers_type;
    typedef fcgi::spec_detail::role_types     role_type;

    /// Construct
    basic_client()
      : request_id_(-1)
      , status_(none_)
      , keep_connection_(false)
      , total_sent_bytes_(0)
      , total_sent_packets_(0)
      , header_()
      , outbuf_()
    {
    }

    /// Construct
    basic_client(io_service_type& ios)
      : request_id_(-1)
      , status_(none_)
      , keep_connection_(false)
      , total_sent_bytes_(0)
      , total_sent_packets_(0)
      , header_()
      , outbuf_()
    {
    }

    /// Destroy
    /** Closing the connection as early as possible is good for efficiency */
    ~basic_client()
    {
      close();
    }

    /// Construct the client by claiming a request id.
    /**
     * Before loading a request, it will usually not have a request id. This
     * function reads headers (and corresponding bodies if necessary) until
     * a BEGIN_REQUEST record is found. The calling request then claims and
     * serves that request.
     */
    template<typename RequestImpl>
    boost::system::error_code
      construct(RequestImpl& req, boost::system::error_code& ec)
    {
      status_ = constructed;
      return ec;
    }

    bool is_open() const
    {
      return connection_->is_open();
    }

    void close(boost::uint64_t app_status = 0)
    {
      boost::system::error_code ec;
      close(app_status, ec);
      detail::throw_error(ec);
    }

    boost::system::error_code
      close(boost::uint64_t app_status, boost::system::error_code& ec)
    {
      // We can safely ignore this next bit when the connection
      // is already closed. This might happen when the client aborts
      // the connection.
      if (!ec && status_ != closed_ && connection_->is_open())
      {
        outbuf_.clear();
        header_.reset(fcgi::spec_detail::END_REQUEST, request_id_, 8);

        // Write an EndRequest packet to the server.
        fcgi::spec::end_request_body body(
          app_status, fcgi::spec_detail::REQUEST_COMPLETE);

        outbuf_.push_back(header_.data());
        outbuf_.push_back(body.data());

        write(*connection_, outbuf_, boost::asio::transfer_all(), ec);

        if (!ec && !keep_connection_)
        {
          connection_->close();
        }
      }
      
      if (ec && !keep_connection_)
      {
        // If there was an error writing to the client, we can ignore it
        // unless the `keep_connection_` flag is set.
        // The client has likely aborted the request if we reach here.
        ec = boost::system::error_code();
      }
      return ec;
    }

    /// Associate a connection with this client
    /**
     * Note: the connection must have been created using the new operator
     */
    bool set_connection(connection_type* conn)
    {
      // make sure there isn't already a connection associated with the client
      //if (!connection_) return false;
      connection_.reset(conn);
      return true;
    }

    /// Associate a connection with this client
    bool set_connection(const connection_type::pointer& conn)
    {
      // make sure there isn't already a connection associated with the client
      //if (!connection_) return false;
      connection_ = conn;
      return true;
    }

    /// Get a shared_ptr of the connection associated with the client.
    connection_type::pointer&
    connection() { return connection_; }

    /// Write some data to the client.
    template<typename ConstBufferSequence>
    std::size_t 
    write_some(const ConstBufferSequence& buf, boost::system::error_code& ec)
    {
      typename ConstBufferSequence::const_iterator iter = buf.begin();
      typename ConstBufferSequence::const_iterator end  = buf.end(); 

      outbuf_.clear();
      outbuf_.push_back(boost::asio::buffer(header_.data()));

      int total_buffer_size(0);
      for(; iter != end; ++iter)
      {
        boost::asio::const_buffer buffer(*iter);
        std::size_t new_buf_size( boost::asio::buffer_size(*iter) );
        if (total_buffer_size + new_buf_size 
             > static_cast<std::size_t>(fcgi::spec::max_packet_size::value))
        {
          // If the send buffer is empty, extract a chunk of the
          // new buffer to send. If there is already some data
          // ready to send, don't add any more data to the pack.
          if (total_buffer_size == 0)
          {
            total_buffer_size
              = std::min<std::size_t>(new_buf_size,65500);
            /*
            std::cerr<< "Oversized buffer: " << total_buffer_size
                     << " / " << new_buf_size << " bytes sent\n";
            */
            outbuf_.push_back(
              boost::asio::buffer(*iter, total_buffer_size));
            break;
          }
          else
            break;
        }
        else
        {
          total_buffer_size += new_buf_size;
          outbuf_.push_back(*iter);
        }
      }
      header_.reset(fcgi::spec_detail::STDOUT, request_id_, total_buffer_size);
      
      std::size_t bytes_transferred
        = boost::asio::write(*connection_, outbuf_
                            , boost::asio::transfer_all(), ec);

      
      std::cerr<< "Transferred " << bytes_transferred
               << " / " << total_buffer_size << " bytes (running total: "
               << total_sent_bytes_ << " bytes; "
               << total_sent_packets_ << " packets).\n";
      if (ec)
        std::cerr<< "Error " << ec << ": " << ec.message() << '\n';
      
      total_sent_bytes_ += bytes_transferred;
      total_sent_packets_ += 1;
      // Now remove the protocol overhead from the caller, who
      // doesn't care about them.
      bytes_transferred -= fcgi::spec::header_length::value;
      // Check everything was written ok.
      if (!ec && bytes_transferred != total_buffer_size)
        ec = ::BOOST_CGI_NAMESPACE::fcgi::error::couldnt_write_complete_packet;

      return bytes_transferred;
    }

    /// Read data into the supplied buffer.
    /**
     * Reads some data that, correctly checking and stripping FastCGI headers.
     *
     * Returns the number of bytes read and sets `ec` such that `ec` evaluates
     * to `true` iff an error occured during the read operation.
     *
     * Notable errors:
     * - `fcgi::error::data_for_another_request`
     * - `fcgi::error::connection_locked`
     *
     * These must be dealt with by user code if they choose to read through the
     * client (reading through the request is recommended).
     */
    template<typename MutableBufferSequence>
    std::size_t
    read_some(const MutableBufferSequence& buf, boost::system::error_code& ec)
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


    /// Get the status of the client.
    const client_status& status() const
    {
      return status_;
    }

    /// Set the status of the client.
    void status(client_status status)
    {
      status_ = status;
    }

    bool keep_connection() const
    {
      return keep_connection_;
    }

    std::size_t& bytes_left()
    {
      return bytes_left_;
    }

    boost::uint16_t const& request_id() const
    {
      return request_id_;
    }

  public:
    friend class fcgi_request_service;
    boost::uint16_t request_id_;
    client_status status_;
    std::size_t bytes_left_;    
    connection_ptr   connection_;
    
    boost::uint64_t total_sent_bytes_;
    boost::uint64_t total_sent_packets_;

    /// Buffer used to check the header of each packet.
    //header_buffer_type out_header_;
    fcgi::spec::header header_;

    /// Output buffer.
    std::vector<boost::asio::const_buffer> outbuf_;

    bool keep_connection_;
    role_type role_;

  };

 } // namespace common

namespace fcgi {
    typedef
      common::basic_client<
        connections::shareable_tcp, ::BOOST_CGI_NAMESPACE::common::fcgi_
      >
    client;
} // namespace fcgi

BOOST_CGI_NAMESPACE_END

#endif // CGI_FCGI_CLIENT_HPP_INCLUDED__

