#ifndef CGI_PROTOCOL_TRAITS_HPP_INCLUDED__
#define CGI_PROTOCOL_TRAITS_HPP_INCLUDED__

namespace cgi { namespace detail {
  
  template<typename Protocol>
  struct protocol_traits
  {
    typedef Protocol                         protocol_type;
    typedef basic_request<Protocol>          request_type;
    typedef gateway<Protocol>                gateway_type;
    typedef basic_service<Protocol>          service_type;
    typedef service_options<Protocol>        service_options; // just an idea
  };

} // namespace detail
} // namespace cgi

#endif // CGI_PROTOCOL_TRAITS_HPP_INCLUDED__
