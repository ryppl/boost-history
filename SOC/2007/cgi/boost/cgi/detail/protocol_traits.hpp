//                -- protocol_traits.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REQUEST_TRAITS_HPP_INCLUDED__
#define CGI_REQUEST_TRAITS_HPP_INCLUDED__

#include "../tags.hpp"
#include "../basic_connection_fwd.hpp"

namespace cgi {

  // Forward declarations

  class cgi_request_impl;
  class acgi_request_impl;
  class async_cgi_request_impl;
  class fcgi_request_impl;
  class scgi_request_impl;

  //template<typename>
  class cgi_service_impl;
  class acgi_service_impl;
  class async_cgi_service_impl;
  class fcgi_service_impl;
  class scgi_service_impl;

  class cgi_gateway_impl;
  class acgi_gateway_impl;
  class async_cgi_gateway_impl;
  class fcgi_gateway_impl;
  class scgi_gateway_impl;

  class cgi_gateway_service;
  class acgi_gateway_service;
  class async_cgi_gateway_service;
  class fcgi_gateway_service;
  class scgi_gateway_service;
  template<typename> class gateway_service;


 namespace detail {

   template<typename Protocol>
    struct protocol_traits
    {
    };

    template<>
    struct protocol_traits<tags::cgi>
    {
      typedef cgi_request_impl                       impl_type;
      typedef cgi_service_impl                       service_impl_type;
      typedef basic_connection<tags::stdio>          connection_type;
//    typedef cgi_gateway_impl                       gateway_impl_type;
//    typedef cgi_gateway_service                    gateway_service_impl_type;
    };

    template<>
    struct protocol_traits<tags::async_cgi>
    {
      typedef async_cgi_request_impl                 impl_type;
      typedef async_cgi_service_impl                 service_impl_type;
      typedef basic_connection<tags::async_stdio>    connection_type;
      typedef async_cgi_gateway_impl                 gateway_impl_type;
      typedef async_cgi_gateway_service              gateway_service_type;
    };

    template<>
    struct protocol_traits<tags::acgi>
    //  : protocol_traits<tags::async_cgi>
    {
      typedef acgi_request_impl                      impl_type;
      typedef acgi_service_impl                      service_impl_type;
      typedef basic_connection<tags::async_stdio>    connection_type;
      typedef acgi_gateway_impl                      gateway_impl_type;
      typedef acgi_gateway_service                   gateway_service_type;
    };

    template<>
    struct protocol_traits<tags::fcgi>
    {
      typedef fcgi_request_impl                      impl_type;
      typedef fcgi_service_impl                      service_impl_type;
      typedef basic_connection<tags::tcp_socket>     connection_type;
      typedef fcgi_gateway_impl                      gateway_impl_type;
      typedef fcgi_gateway_service                   gateway_service_type;
    };

    template<>
    struct protocol_traits<tags::scgi>
    {
      typedef scgi_request_impl                      impl_type;
      typedef scgi_service_impl                      service_impl_type;
      typedef basic_connection<tags::tcp_socket>     connection_type;
      typedef scgi_gateway_impl                      gateway_impl_type;
      typedef scgi_gateway_service                   gateway_service_type;
    };

 } // namespace detail
} // namespace cgi

#endif // CGI_REQUEST_TRAITS_HPP_INCLUDED__
