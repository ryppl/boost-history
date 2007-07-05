//                -- basic_connection.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_CONNECTION_HPP_INCLUDED__
#define CGI_BASIC_CONNECTION_HPP_INCLUDED__

#include <istream>
#include <ostream>
#include <boost/asio.hpp>
#include "connection_base.hpp"

namespace cgi {

  template<typename ProtocolService, typename ConnectionType>
  class basic_connection
  : connection_base
  //, connection_wrapper<ConnectionType>
  {
  public:
    typedef connection_impl<ProtocolService::protocol_type>  impl_type;

  private:
    impl_type* impl_;
  };

  template<typename ProtocolService>
  class basic_connection<tags::stdio>
    : public connection_base
  {
  public:
    typedef ProtocolService   protocol_service_type;

    basic_connection(protocol_service_type& ps)
      : in_(std::cin)
      , out_(std::cout)
    {
    }

    template<typename MutableBufferSequence>
    std::size_t read(MutableBufferSequence buf, boost::system::error_code& ec)
    {
      if( buf.data() != in_.rdbuf() )
        return in_.read(buf.data(), buf.size());
      return buf.size();
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read(MutableBufferSequence buf, Handler handler)
    {
      handler(boost::system_error(), read(buf));
    }

    template<typename ConstBufferSequence>
    std::size_t write(ConstBufferSequence& buf)
    {
      return out_.write(buf.data(), buf.size());
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write(ConstBufferSequence& buf, Handler handler)
    {
      out_.write(buf.data(), buf.size());
      protocol_service_.post(handler);
    }
      
  private:
    protocol_service_type& protocol_service_;
    std::istream& in_;
    std::ostream& out_;
  };

  template<typename ProtocolService>
  class basic_connection<tags::stdio_async>
    : public connection_base
  {
  public:
    typedef ProtocolService   protocol_service_type;

    basic_connection(protocol_service_type& ps)
      : protocol_service_(ps)
      , in_(std::cin)
      , out_(std::cout)
    {
    }

    template<typename MutableBufferSequence>
    std::size_t read(MutableBufferSequence buf)
    {
      if( buf.data() != in_.rdbuf() )
        return in_.read(buf.data(), buf.size());
      return buf.size();
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read(MutableBufferSequence buf, Handler handler)
    {
      std::size_t bytes_read;
      if( buf.data() != in_.rdbuf() )
        bytes_read = in_.read(buf.data(), buf.size());
      else
        bytes_read = buf.size();

      protocol_service_.post(boost::bind(&Handler
                                        , handler
                                        , boost::system::system_error()
                                        , bytes_read ));
    }

    template<typename ConstBufferSequence>
    std::size_t write(ConstBufferSequence& buf)
    {
      return out_.write(buf.data(), buf.size());
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write(ConstBufferSequence& buf, Handler handler)
    {
      out_.write(buf.data(), buf.size());
      protocol_service_.post(handler);
    }
      
  private:
    protocol_service_type& protocol_service_;
    std::istream& in_;
    std::ostream& out_;
  };



  template<typename ProtocolService>
  class basic_connection<ProtocolService, tags::tcp_socket>
    : public connection_base
  {
  public:
    typedef ProtocolService   protocol_service_type;

    basic_connection(protocol_service_type& ps)
      : protocol_service_(ps)
      , sock_(ps.io_service())
    {
    }

    template<typename ConstBufferSequence, typename Handler>
    void async_write(ConstBufferSequence& buf, Handler handler)
    {
    }

  private:
    protocol_service_type protocol_service_;
    boost::asio::ip::tcp::socket sock_;
  };


  typedef basic_connection<tags::stdio>    stdio_connection;
  typedef basic_connection<tags::tcp>      tcp_connection;
  //typedef basic_connection<tags::pipe>     pipe_connection; // not implemented


} // namespace cgi

#endif // CGI_BASIC_CONNECTION_HPP_INCLUDED__
