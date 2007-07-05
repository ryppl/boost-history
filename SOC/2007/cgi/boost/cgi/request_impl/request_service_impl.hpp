#ifndef CGI_REQUEST_SERVICE_IMPL_HPP_INCLUDED__
#define CGI_REQUEST_SERVICE_IMPL_HPP_INCLUDED__

namespace cgi {

  template<typename Protocol>
  class request_service_impl
  {
    typedef request_service_impl    type;
  };

  /////////////////////////////////////////////////////////////////////////////
  // FastCGI traits
  template<>
  class request_service_impl<protocol::fcgi>
  {
  public:
    typedef fcgi_request_service_impl    type;
  };

  /////////////////////////////////////////////////////////////////////////////
  // CGI traits
  template<>
  class request_service_impl<protocol::cgi, false>
  {
  public:
    typedef cgi_request_service_impl    type;
  };

  //template<>
  //class request_service_impl<protocol::cgi, request_type::responder, true>
  //{
  //public:
  //  typedef cgi_arequest_service_impl    type;
  //};

  /////////////////////////////////////////////////////////////////////////////
  // SCGI traits
  //template<>
  //class request_service_impl<protocol::scgi>
  //{
  //public:
  //  typedef scgi_request_service_impl    type;
  //};

} // namespace cgi

#endif // CGI_REQUEST_SERVICE_IMPL_HPP_INCLUDED__
