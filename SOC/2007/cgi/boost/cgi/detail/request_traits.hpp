#ifndef CGI_REQUEST_TRAITS_HPP_INCLUDED__
#define CGI_REQUEST_TRAITS_HPP_INCLUDED__

#include "request_type.hpp"

namespace cgi {
 namespace detail {
  
    template<typename Protocol, typename RequestType = request_type::responder>
    struct request_traits
    {
    };

    template<>
    struct request_traits<protocol::cgi>
    {
      typedef cgi_request_impl                  impl_type;
      //typedef request_service<protocol::cgi>    service_type;
      typedef cgi_service_impl                  service_impl_type;
    };

    template<>
    struct request_traits<protocol::async_cgi>
    {
      typedef async_cgi_request_impl                  impl_type;
      //typedef request_service<protocol::async_cgi>    service_type;
      typedef async_cgi_service_impl                  service_impl_type;
    };

    template<>
    struct request_traits<protocol::fcgi>
    {
      typedef fcgi_request_impl                  impl_type;
      //typedef request_service<protocol::fcgi>    service_type;
      typedef fcgi_service_impl                  service_impl_type;
    };

    template<>
    struct request_traits<protocol::scgi>
    {
      typedef scgi_request_impl                  impl_type;
      //typedef request_service<protocol::scgi>    service_type;
      typedef scgi_service_impl                  service_impl_type;
    };

 } // namespace detail
} // namespace cgi

#endif // CGI_REQUEST_TRAITS_HPP_INCLUDED__
