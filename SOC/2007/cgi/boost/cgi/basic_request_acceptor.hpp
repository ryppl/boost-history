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

#include "basic_io_object.hpp"
#include "is_async.hpp"

namespace cgi {

  template<typename RequestAcceptorService>
  class basic_request_acceptor
    : private boost::noncopyable
    , public basic_io_object<RequestAcceptorService, is_async<RequestAcceptorService::protocol_type>::value>
  {
  public:
    //  typedef impl_type;
    typedef RequestAcceptorService         service_type;
    typedef service_type::protocol_type    protocol_type;

    explicit basic_request_acceptor(basic_protocol_service<protocol_type>& s)
      : boost::asio::use_service<RequestAcceptorService>(s.io_service())
    {
    }

    ~basic_request_acceptor()
    {
      this->service.cancel();
    }

    template<typename CommonGatewayRequest>
    void accept(CommonGatewayRequest& request)
    {
      boost::system::error_code ec;
      this->service.accept(request, ec);
      boost::throw_error(ec);
    }

    template<typename CommonGatewayRequest> boost::system::error_code&
    accept(CommonGatewayRequest& request, boost::system::error_code& ec)
    {
      return this->service.accept(request, ec);
    }

    template<typename CommonGatewayRequest, typename Handler>
    void async_accept(CommonGatewayRequest& request, Handler handler)
    {
      this->service.async_accept(request, handler);
    }

  private:
    service_type& service_;
  };



  /*
  /// Specialisation for the CGI protocol
  template<>
  class basic_acceptor<protocol::cgi>
    : private boost::noncopyable
  {
  public:
  /// Accept a connection
  /*
   * For CGI, the connection is just a std::cin/cout wrapper, so is always
   * connected
   
  void accept() { }
  
  /// Asynchronously accept a connection
  /*
   * Ask the service to asynchronously accept a connection. When one is accepted
   * the handler is invoked (in a thread calling basic_service<>::run() ?).
   
  template<typename Handler>
  void async_accept(Handler handler)
  {
    handler();
  }
     */
} // namespace cgi

#endif // CGI_BASIC_REQUEST_ACCEPTOR_HPP_INCLUDED__
