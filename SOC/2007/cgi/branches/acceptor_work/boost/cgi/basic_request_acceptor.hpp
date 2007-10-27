//            -- basic_request_acceptor.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_BASIC_REQUEST_ACCEPTOR_HPP_INCLUDED__
#define CGI_BASIC_REQUEST_ACCEPTOR_HPP_INCLUDED__

#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

#include <boost/asio/basic_io_object.hpp>
#include "boost/cgi/detail/throw_error.hpp"

namespace cgi {

  /// The interface class for any *cgi::acceptor.
  template<typename RequestAcceptorService>
  class basic_request_acceptor
    : public boost::asio::basic_io_object<RequestAcceptorService>
    //, private boost::noncopyable
  {
  public:
    //  typedef impl_type;
    typedef RequestAcceptorService         service_type;
    typedef typename service_type::protocol_type    protocol_type;
    typedef int                            port_number_type;

    explicit basic_request_acceptor(basic_protocol_service<protocol_type>& ps)
      : boost::asio::basic_io_object<RequestAcceptorService>(ps.io_service())
    {
      this->service.set_protocol_service(this->implementation, s);
    }

    explicit basic_request_acceptor(basic_protocol_service<protocol_type>& ps
                                   , port_number_type port_num)
      : boost::asio::basic_io_object<RequestAcceptorService>(ps.io_service())
    {
      this->service.set_protocol_service(this->implementation, ps);
    }

    ~basic_request_acceptor()
    {
    }

    /// Check if the acceptor is open
    bool is_open()
    {
      return this->service.is_open(this->implementation);
    }

    /// Open the acceptor
    template<typename Protocol>
    void open(Protocol& protocol)
    {
      boost::system::error_code ec;
      this->service.open(this->implementation, protocol, ec);
      detail::throw_error(ec);
    }

    /// Open the acceptor
    template<typename Protocol>
    boost::system::error_code
      open(Protocol& protocol, boost::system::error_code& ec)
    {
      return this->service.open(this->implementation, protocol, ec);
    }

    /// Cancel all asynchronous operations associated with the acceptor.
    boost::system::error_code
      cancel()
    {
      return this->service.cancel(this->implementation);
    }

    /// Close the acceptor
    void close()
    {
      boost::system::error_code ec;
      this->service.close(this->implementation, ec);
      detail::throw_error(ec);
    }

    /// Close the acceptor
    boost::system::error_code
      close(boost::system::error_code& ec)
    {
      return this->service.close(this->implementation, ec);
    }

    /// Accept one request
    template<typename CommonGatewayRequest>
    void accept(CommonGatewayRequest& request)
    {
      boost::system::error_code ec;
      this->service.accept(this->implementation, request, ec);
      detail::throw_error(ec);
    }

    /// Accept one request
    template<typename CommonGatewayRequest>
    boost::system::error_code&
    accept(CommonGatewayRequest& request, boost::system::error_code& ec)
    {
      return this->service.accept(this->implementation, request, ec);
    }

    /// Asynchronously accept one request
    template<typename CommonGatewayRequest, typename Handler>
    void async_accept(CommonGatewayRequest& request, Handler handler)
    {
      this->service.async_accept(this->implementation, request, handler);
    }
  };

} // namespace cgi

#endif // CGI_BASIC_REQUEST_ACCEPTOR_HPP_INCLUDED__
