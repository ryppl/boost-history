//           -- async_stdio_connection_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ASYNC_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
#define CGI_ASYNC_STDIO_CONNECTION_IMPL_HPP_INCLUDED__

namespace cgi {

 template<typename ProtocolService>
  class basic_connection<tags::stdio_async, ProtocolService>
    : public connection_base
    , public basic_connection<ProtocolService, tags::stdio>
  {
  public:
    basic_connection(protocol_service_type& ps)
      : protocol_service_(ps)
      , basic_connection<tags::stdio, ProtocolService>()
    {
    }

    template<typename MutableBufferSequence, typename Handler>
    void async_read_some(MutableBufferSequence buf, boost::system::error_code& ec
                        , Handler handler)
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

    template<typename ConstBufferSequence, typename Handler>
    void async_write_some(ConstBufferSequence& buf, boost::system::error_code& ec
                         , Handler handler)
    {
      out_.write(buf.data(), buf.size());
      protocol_service_.post(handler);
    }
      
  private:
    protocol_service_type& protocol_service_;
  };

  template<typename ProtocolService = detail::async_cgi_service>
  struct async_stdio_connection
  {
    typedef basic_connection<tags::async_cgi, ProtocolService>    type;
  };      

} // namespace cgi

#endif // CGI_ASYNC_STDIO_CONNECTION_IMPL_HPP_INCLUDED__
