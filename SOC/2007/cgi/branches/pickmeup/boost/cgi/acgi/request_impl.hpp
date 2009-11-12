//              -- acgi_request_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ACGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_ACGI_REQUEST_IMPL_HPP_INCLUDED__

#include "boost/cgi/detail/push_options.hpp"

#include "boost/cgi/acgi/service.hpp"
#include "boost/cgi/basic_client.hpp"
#include "boost/cgi/connections/async_stdio.hpp"
#include "boost/cgi/detail/cgi_request_impl_base.hpp"

// Make this ProtocolService-independent

BOOST_CGI_NAMESPACE_BEGIN
 namespace acgi {

  class request_impl
    : public detail::cgi_request_impl_base<connections::async_stdio>
  {
  public:
    typedef ::BOOST_CGI_NAMESPACE::acgi::service protocol_service_type;
    typedef protocol_service_type::protocol_type protocol_type;
    typedef connections::async_stdio             connection_type;
    typedef
      ::BOOST_CGI_NAMESPACE::common::basic_client<
        connection_type, common::tags::acgi
      >
    client_type;

    request_impl()
      : detail::cgi_request_impl_base<connection_type>()
    {
    }

    protocol_service_type* service_;
  };

 } // namespace acgi
BOOST_CGI_NAMESPACE_END

#include "boost/cgi/detail/pop_options.hpp"

#endif // CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__

