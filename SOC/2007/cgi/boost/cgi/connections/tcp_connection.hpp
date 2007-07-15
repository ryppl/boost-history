//               -- tcp_connection_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_TCP_CONNECTION_IMPL_HPP_INCLUDED__
#define CGI_TCP_CONNECTION_IMPL_HPP_INCLUDED__

namespace cgi {

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

  template<typename ProtocolService = detail::fcgi_service>
  struct tcp_connection
  {
    typedef basic_connection<tags::tcp_socket, ProtocolService>    type;
  };

} // namespace cgi

#endif // CGI_TCP_CONNECTION_IMPL_HPP_INCLUDED__
