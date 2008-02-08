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

#include <boost/shared_ptr.hpp>
#include <boost/logic/tribool.hpp>
#include "boost/cgi/tags.hpp"
#include "boost/cgi/map.hpp"
#include "boost/cgi/io_service.hpp"
#include "boost/cgi/basic_client.hpp"
#include "boost/cgi/connections/shareable_tcp_socket.hpp"
#include "boost/cgi/fcgi/specification.hpp"
#include "boost/cgi/read.hpp"
#include "boost/cgi/buffer.hpp"
#include "boost/cgi/error.hpp"
//#include "boost/cgi/fcgi/request_fwd.hpp"
#include "boost/cgi/detail/protocol_traits.hpp"
//#error BOOST_HAS_RVALUE_REFS
namespace cgi {

  /// A client that uses a TCP socket that owned by it.
  template<typename Protocol>
  class basic_client<shareable_tcp_connection, Protocol>
  {
  public:
    typedef ::cgi::io_service                 io_service_type;
    typedef ::cgi::map                        map_type;
    typedef Protocol                          protocol_type;
    typedef shareable_tcp_connection          connection_type;
    //typedef detail::protocol_traits<
    //  fcgi_
    //>::request_impl_type                      request_impl_type;
    //struct connection_type : shareable_tcp_connection
    //{ typedef boost::shared_ptr<connection_type> pointer; }
    typedef typename connection_type::pointer          connection_ptr;
    typedef boost::array<
        unsigned char
      , fcgi::spec::header_length::value
    >                                         header_buffer_type;
    typedef boost::asio::mutable_buffers_1    mutable_buffers_type;

    /// Construct
    basic_client()
      : request_id_(-1)
      , closed_(false)
    {
    }

    /// Construct
    basic_client(io_service_type& ios)
      : request_id_(-1)
      , closed_(false)
      //, io_service_(ios)
      //, connection_(new connection_type::pointer(ios))
    {
    }

    /// Destroy
    /** Closing the connection as early as possible is good for efficiency */
    ~basic_client()
    {
      if (!keep_connection_ && connection_)
        connection_->close();
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
      BOOST_ASSERT(connection_->is_open());

      for(;;)
      {
        if (read_header(ec))
          break;
        int id(fcgi::spec::get_request_id(header_buf_));
        if (id == fcgi::spec::null_request_id::value)
          handle_admin_request();
        else
        if (fcgi::spec::get_type(header_buf_) 
            == fcgi::spec::begin_request::value)
        {
          request_id_ = id;
          if (read_header(ec))
            break;
          req.request_role_ = fcgi::spec::begin_request::get_role(header_buf_);
          std::cerr<< "[hw] New request role: " << req.request_role_
              << " (" << fcgi::spec::role_type::to_string(header_buf_) << ")"
              << std::endl;
          keep_connection_
            = fcgi::spec::begin_request::get_flags(header_buf_)
              & fcgi::spec::keep_connection;
          break;
        }else
          handle_other_request_header();
      }

      return ec;
    }

    bool is_open() const
    {
      return connection_->is_open();
    }

    void close()
    {
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
      //detail::make_header(out_header_, buf
      return connection_->write_some(buf, ec);
    }

    /// Read some data from the client.
    template<typename MutableBufferSequence>
    std::size_t
      read_some(const MutableBufferSequence& buf, boost::system::error_code& ec)
    {
      if (closed_)
        return 0;

      if (read_header(ec))
        return -1;

      boost::tribool state = parse_header();
      std::size_t bytes_read;//( connection_->read_some(buf, ec) );
      return bytes_read;
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

    boost::system::error_code
      read_header(boost::system::error_code& ec)
    {
      header_buf_ = header_buffer_type();
      if (8 != read(*connection_, buffer(header_buf_)
                   , boost::asio::transfer_all(), ec) || ec)
        return ec;
      
      //if (ec) return ec;

      std::cerr<< std::endl
          << "[hw] Header details {" << std::endl
          << "  RequestId := " << fcgi::spec::get_request_id(header_buf_) << std::endl
          << "  FastCGI version := " << fcgi::spec::get_version(header_buf_) << std::endl
          << "  Type := " << fcgi::spec::get_type(header_buf_)
          << " (" << fcgi::spec::request_type::to_string(header_buf_) << ")" << std::endl
          << "  Content-length := " << fcgi::spec::get_content_length(header_buf_) << std::endl
          << "}" << std::endl;

      return ec;
    }

  private:
    friend class fcgi_request_service;
    boost::uint16_t request_id_;
    //request_impl_type* current_request_;
    
    /// A marker to say if the final STDIN (and/or DATA) packets have been
    // read. Note: having data on the connection doesn't imply it's for
    // this request; we can save time by knowing when to not even try.
    bool closed_;

    connection_ptr   connection_;

    //fcgi::spec_detail::Header hdr_;
    /// Buffer used to check the header of each packet.
    header_buffer_type header_buf_; //in_header_, out_header_;

    bool keep_connection_;

    /*** Various handlers go below here; they might find a
     * better place to live ***/

    // **FIXME**
    void handle_admin_request()
    {
      std::cerr<< std::endl << "**FIXME** " << __FILE__ << ":" << __LINE__ 
        << " handle_admin_request()" << std::endl;
    }

    // **FIXME**
    void handle_other_request_header()
    {
      std::cerr<< std::endl << "**FIXME** " << __FILE__ << ":" << __LINE__ 
        << " handle_other_request_header()" << std::endl;
    }

    // **FIXME**    
    boost::system::error_code
      process_begin_request(boost::uint16_t id, const unsigned char* buf
                           , boost::uint16_t, boost::system::error_code& ec)
    {
      if (read_header(ec))
        detail::throw_error(ec);

      std::cerr<< "Role: " << fcgi::spec::begin_request::get_role(header_buf_) << std::endl;
      //connection_->request_map_[id] = 
      return ec;
    }

    boost::system::error_code
      process_abort_request(boost::uint16_t id, const unsigned char* buf
                           , boost::uint16_t, boost::system::error_code& ec)
    {
/*
      connection_type::request_map_type::iterator i
        = connection_->find(id);

      if (i == connection_type::request_map_type::iterator())
      {
        return bad_request_id;
      }

      //lookup_request(id).abort();
*/
      return ec;
    }

    boost::system::error_code
      process_params(boost::uint16_t id, const unsigned char* buf
                    , boost::uint16_t len, boost::system::error_code& ec)
    {
      if (0 == len)
      { // This is the final param record.
        
        // **FIXME**
        std::cerr<< "[hw] Final PARAM record found." << std::endl;
        return ec;
      }

      while(len)
      {
        boost::uint32_t   name_len, data_len;
        std::string name, data;
        if (*buf >> 7 == 0)
        {
            name_len = *(buf++);
            --len;
        }
        else
        {
            name_len = ((buf[0] & 0x7F) << 24)
                     +  (buf[1]         << 16)
                     +  (buf[2]         <<  8)
                     +   buf[3];
            buf += 4;
            len -= 4;
        }

        if (*buf >> 7 == 0)
        {
            data_len = *(buf++);
            --len;
        }
        else
        {
            data_len = ((buf[0] & 0x7F) << 24)
                     +  (buf[1]         << 16)
                     +  (buf[2]         <<  8)
                     +   buf[3];
            buf += 4;
            len -= 4;
        }
        name.assign(reinterpret_cast<const char*>(buf), name_len);
        data.assign(reinterpret_cast<const char*>(buf)+name_len, data_len);
        buf += (name_len + data_len);
        len -= (name_len + data_len);

        std::cerr<< "[hw] name := " << name << std::endl;
        std::cerr<< "[hw] data := " << data << std::endl;
      }

      return ec;
    }


    boost::system::error_code
      process_stdin(boost::uint16_t id, const unsigned char* buf
                   , boost::uint16_t len, boost::system::error_code& ec)
    {
      if (0 == len)
      {
        closed_ = true;

        // **FIXME**
        std::cerr<< "[hw] Final STDIN record found." << std::endl;
        return ec;
      }

      // **FIXME**
      std::cerr<< "[hw] Found some STDIN stuff." << std::endl;
      return ec;
    }

    /// Parse the current header
    boost::tribool parse_header()
    {
      BOOST_ASSERT(fcgi::spec::get_version(header_buf_) == 1
                   && "This library is only compatible with FastCGI 1.0");

      using namespace fcgi::spec_detail;

      switch(fcgi::spec::get_type(header_buf_))
      {
      case BEGIN_REQUEST:
      case PARAMS:
      case STDIN:
      case DATA:
      case GET_VALUES:
        return boost::indeterminate;
      case ABORT_REQUEST:
        return false;
      case UNKNOWN_TYPE:
      default:
        return true;
      }
    }

  public:

    //template<typename Request>
    //Request& lookup_request(boost::uint16_t id)
    //{


    // Mammoth typedef corresponding to function signature of process_*
    // functions.
    typedef boost::system::error_code
      ( ::cgi::basic_client<shareable_tcp_connection, fcgi_>::* proc_func_t)
      (boost::uint16_t, const unsigned char*, boost::uint16_t
      , boost::system::error_code&);

    static const proc_func_t proc_funcs[];
  
    template<typename RequestImpl>
    boost::system::error_code
      parse_packet(RequestImpl& req, boost::system::error_code& ec)
    {
      //current_request_ = &req;

      if (read_header(ec))
        return ec;

      boost::tribool state = parse_header();

      if (state)
      { // the header has been handled and all is ok; continue.
        return ec;
      }else
      if (!state)
      { // The header is confusing; something's wrong. Abort.
        std::cerr<< "Bad header received (this isn't implemented properly yet"
            << std::endl;
        return error::bad_header_type;
      }
      // else route (ie. state == boost::indeterminate)

      std::cerr<< "Got to read more stuff now I think." << std::endl;
      mutable_buffers_type buf
        = req.prepare(fcgi::spec::get_length(header_buf_));

      if (read_body(buf, ec))
        return ec;

      parse_body(buf, ec);

      return ec;
    }

    /// Read the body of the current packet; do nothing with it.
    template<typename MutableBuffersType>
    boost::system::error_code
      read_body(const MutableBuffersType& buffer
               , boost::system::error_code& ec)
    {
      std::size_t bytes_read
        = read(*connection_, buffer, boost::asio::transfer_all(), ec);

      BOOST_ASSERT(bytes_read == fcgi::spec::get_length(header_buf_)
                   && "Couldn't read all of the record body.");
      return ec;
    }

    template<typename MutableBuffersType>
    boost::system::error_code
      parse_body(const MutableBuffersType& buffer
                , boost::system::error_code& ec)
    {
      return //ec;/*
        (this->* proc_funcs[fcgi::spec::get_type(header_buf_)])
            (fcgi::spec::get_request_id(header_buf_)
            , boost::asio::buffer_cast<unsigned char*>(buffer)
            , boost::asio::buffer_size(buffer), ec);
            //*/
    }
  };

//#include "boost/cgi/fcgi/client_fwd.hpp"

namespace fcgi {
    typedef basic_client<shareable_tcp_connection, ::cgi::fcgi_> client;
} // namespace fcgi


  template<>
  const fcgi::client::proc_func_t fcgi::client::proc_funcs[] =
    { 0
    , &fcgi::client::process_begin_request
    , &fcgi::client::process_abort_request
    , 0
    , &fcgi::client::process_params
    , &fcgi::client::process_stdin
    , 0
    , 0
    };

}// namespace cgi

#endif // CGI_FCGI_CLIENT_HPP_INCLUDED__

