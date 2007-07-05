#ifndef CGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_REQUEST_IMPL_HPP_INCLUDED__

namespace cgi {

  template<typename Protocol, typename RequestType = request_type::responder>
  class request_impl
  {
  };

  /////////////////////////////////////////////////////////////////////////////
  // FastCGI traits
  template<>
  class request_impl<protocol::fcgi>
  {
  public:
    typedef fcgi_responder_request_impl    type;
  };

  template<>
  class request_impl<protocol::fcgi, request_type::filter>
  {
  public:
    typedef fcgi_filter_request_impl    type;
  };

  template<>
  class request_impl<protocol::fcgi, request_type::authorizer>
  {
  public:
    typedef fcgi_authorizer_request_impl    type;
  };

  /////////////////////////////////////////////////////////////////////////////
  // CGI traits
  template<>
  class request_impl<protocol::cgi>
  {
  public:
    typedef cgi_request_impl    type;
  };

  template<>
  class request_impl<protocol::async_cgi>
  {
  public:
    typedef cgi_async_request_impl    type;
  };

  /////////////////////////////////////////////////////////////////////////////
  // SCGI traits
  template<>
  class request_impl<protocol::scgi>
  {
  public:
    typedef scgi_request_impl    type;
  };

} // namespace cgi

#endif // CGI_REQUEST_IMPL_HPP_INCLUDED__
