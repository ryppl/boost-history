//           -- async_cgi_request_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__

#include "cgi_service.hpp"
#include "cgi_request_impl.hpp"

// Make this ProtocolService-independent

namespace cgi {

  class async_cgi_request_impl
    : public cgi_request_impl
  {
  public:
    typedef cgi_service    protocol_service_type;

    async_cgi_request_impl(protocol_service_type& protocol_service)
      : cgi_request_impl(protocol_service)
      , protocol_service_(protocol_service)
    {
    }

  protected:
    async_cgi_request_impl(); // private default constructor
    friend class async_cgi_service_impl;

    protocol_service_type& protocol_service_;
  }; 

} // namespace cgi

#endif // CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__
