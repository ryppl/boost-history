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

#include <boost/system.hpp>

namespace cgi {

  template<typename Protocol>
  class basic_request_acceptor
    : private boost::noncopyable
  {
  public:
    //  typedef impl_type;
    typedef Protocol   protocol_type;

    explicit basic_request_acceptor(basic_protocol_service<Protocol>& pserv)
      : service_(pserv) 
    {
    }

    ~basic_request_acceptor()
    {
      service_.cancel();
    }

    template<typename CGI_Request>
    void accept(CGI_Request& request)
    {
      boost::system::error_code ec;
      service_.accept(request, ec);
      boost::throw_error(ec);
    }

    template<typename CGI_Request> boost::system::error_code&
    accept(CGI_Request& request, boost::system::error_code& ec)
    {
      return service_.accept(request, ec);
    }

    template<typename CGI_Request, typename Handler>
    void async_accept(CGI_Request& request, Handler handler)
    {
      service_.async_accept(request, handler);
    }

  private:
    request_acceptor_service<protocol_type> service_;
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
