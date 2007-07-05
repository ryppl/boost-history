#ifndef CGI_CGI_REQUEST_SERVICE_HPP_INCLUDED
#define CGI_CGI_REQUEST_SERVICE_HPP_INCLUDED

namespace cgi {

template<>
class basic_request_service<protocol::cgi>
{
  basic_request_service()
    : in_buf_(std::cin.rdbuf())
    , out_buf_(std::cout.rdbuf())
  {
  }

};

} // namespace cgi

#endif // CGI_CGI_REQUEST_SERVICE_HPP_INCLUDED
