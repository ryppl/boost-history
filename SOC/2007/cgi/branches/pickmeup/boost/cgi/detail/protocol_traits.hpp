//                -- protocol_traits.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
// Wow this is messy. It's not expected to last though; it's
// merely here to make things more separated than they need to
// be, so everything can be meshed together slowly.
// (or something like that)
//
/////////////////////////////////////////////////////////////////
#ifndef CGI_REQUEST_TRAITS_HPP_INCLUDED__
#define CGI_REQUEST_TRAITS_HPP_INCLUDED__

#include <boost/none.hpp>
#include <boost/shared_ptr.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/fwd/basic_protocol_service_fwd.hpp"
#include "boost/cgi/common/tags.hpp"
#include "boost/cgi/common/role_type.hpp"
#include "boost/cgi/fcgi/specification.hpp"
#include "boost/cgi/fwd/basic_request_fwd.hpp"
#include "boost/cgi/fwd/basic_connection_fwd.hpp"

BOOST_CGI_NAMESPACE_BEGIN

  namespace cgi  {}
  namespace fcgi
  {
  class fcgi_request_impl;
  class fcgi_service_impl;
  class fcgi_request_service;
  class fcgi_acceptor_service;
  }
  namespace scgi
  {
  class scgi_request_impl;
  class scgi_service_impl;
  class scgi_request_service;
  class scgi_acceptor_service;
  }

  // Forward declarations

  class cgi_request_impl;
  class fcgi_request_impl;

  class cgi_service_impl;
  class fcgi_service_impl;

  class fcgi_acceptor_service;

  class cgi_request_service;
  class fcgi_request_service;

 namespace detail {

   namespace tags = ::BOOST_CGI_NAMESPACE::common::tags;

   template<typename Protocol>
    struct protocol_traits
    {
    };

    template<>
    struct protocol_traits<tags::cgi>
    {
      typedef protocol_traits<tags::cgi>             type;
      typedef cgi_request_impl                       impl_type;
      typedef cgi_request_service                    request_service_impl;
      typedef common::basic_protocol_service<
                  tags::cgi
              >                                      protocol_service_type;
      typedef common::basic_request<
                  request_service_impl
                , protocol_service_type
              >                                      request_type; 
      typedef cgi_service_impl                       service_impl_type;
      typedef common::basic_connection<tags::async_stdio>  connection_type;
      typedef boost::none_t                          header_type;
      typedef common::role_type                      role_type;
    };

    template<>
    struct protocol_traits<tags::fcgi>
    {
      typedef protocol_traits<tags::fcgi>            type;
      typedef fcgi::fcgi_request_impl                impl_type;
      typedef fcgi::fcgi_request_service             request_service_impl;
      typedef common::basic_protocol_service<
                  common::fcgi_
              >                                      protocol_service_type;
      typedef common::basic_request<
                  request_service_impl
                , protocol_service_type
              >                                      request_type; 
      typedef boost::shared_ptr<request_type>        request_ptr;
      typedef fcgi::fcgi_service_impl                service_impl_type;
      typedef fcgi::fcgi_acceptor_service            acceptor_service_impl;
      typedef common::basic_connection<
                  tags::shareable_tcp_socket
              >                                      connection_type;
      typedef fcgi::spec::header                     header_type;
      typedef fcgi::spec_detail::role_types          role_type;
    };

    template<>
    struct protocol_traits<tags::scgi>
    {
      typedef protocol_traits<tags::scgi>            type;
      typedef scgi::scgi_request_impl                impl_type;
      typedef scgi::scgi_request_service             request_service_impl;
      typedef common::basic_protocol_service<tags::scgi>     protocol_service_type;
      typedef common::basic_request<
        request_service_impl, protocol_service_type
      >                                              request_type; 
      typedef scgi::scgi_service_impl                service_impl_type;
      typedef scgi::scgi_acceptor_service            acceptor_service_impl;
      typedef common::basic_connection<tags::tcp_socket>     connection_type;
    };

    // **FIXME** (remove)
    template<>
    struct protocol_traits< ::BOOST_CGI_NAMESPACE::common::scgi_>
      : protocol_traits<tags::scgi>
    {
    };

    // **FIXME** (remove)
    template<>
    struct protocol_traits< ::BOOST_CGI_NAMESPACE::common::fcgi_>
      : protocol_traits<tags::fcgi>
    {
    };

 } // namespace detail
BOOST_CGI_NAMESPACE_END

#endif // CGI_REQUEST_TRAITS_HPP_INCLUDED__
