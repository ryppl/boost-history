#ifndef CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__

namespace cgi {

  class async_cgi_request_impl
    : public cgi_request_impl
  {
  public:
    //typedef std::map<std::string,std::string>    map_type;

    async_cgi_request_impl(protocol_service_type& protocol_service)
      : cgi_request_impl(protocol_service)
      , protocol_service_(protocol_service)
    {
    }

  protected:
    friend class async_cgi_service_impl;

    protocol_service_type& protocol_service_;
  }; 

} // namespace cgi

#endif // CGI_ASYNC_CGI_REQUEST_IMPL_HPP_INCLUDED__
