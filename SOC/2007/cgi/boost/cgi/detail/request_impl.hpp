#ifndef CGI_REQUEST_IMPL_HPP_INCLUDED
#define CGI_REQUEST_IMPL_HPP_INCLUDED

/// The data structure holding the request details

namespace cgi {

  template<typename CommonGatewayRequest>
  class request_impl
  {
  public:
    typedef std::map<std::string,std::string>  header_type;
    typedef CommonGatewayRequest               request_type;
    typedef request_type::buffer_type          buffer_type;

    request_impl(
    /// Reset the object so it can be used again
    //void reset()
    //{
    //
    //}
    
  private:
    header_type headers_;
    buffer_type stdin_buf_;
  };

} // namespace cgi

#endif // CGI_REQUEST_IMPL_HPP_INCLUDED

