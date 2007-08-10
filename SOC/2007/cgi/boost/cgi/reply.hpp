//                    -- reply.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_REPLY_HPP_INCLUDED__
#define CGI_REPLY_HPP_INCLUDED__

#include <string>

#include "detail/push_options.hpp"
#include "request_ostream.hpp"
#include "buffer.hpp"

namespace cgi {

  /// The reply class: a helper for replying to requests

  // The request_ostream is destined to become a basic_request_ostream
//typedef request_ostream<> reply_;


  class reply
    : public cgi::request_ostream
  {
  public:
    reply(http::status_code sc = http::ok)
      : cgi::request_ostream(sc)
    {
    }
 
    reply(cgi::streambuf* buf, http::status_code sc = http::ok)
      : cgi::request_ostream(buf, sc)
    {
    }
 
    ~reply()
    {
    } 

    /// Some helper functions for the basic CGI 1.1 meta-variables
    void auth_type(const std::string& value)
    {
      std::string str("Auth-type: ");
      str += value;
      this->headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }

    void content_length(const std::string& value)
    {
      std::string str("Content-length: ");
      str += value;
      this->headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }

    /*        void auth_type(const std::string& value)
    {
      std::string str("Auth-type: " + value);
      headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }
 
    void auth_type(const std::string& value)
    {
      std::string str("Auth-type: " + value);
      headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }
 
    void auth_type(const std::string& value)
    {
      std::string str("Auth-type: " + value);
      headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }
 
    void auth_type(const std::string& value)
    {
      std::string str("Auth-type: " + value);
      headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }
 
    void auth_type(const std::string& value)
    {
      std::string str("Auth-type: " + value);
      headers_.push_back(cgi::buffer(str.c_str(), str.size()));
    }
    void content_length(const std::string& value)
    { return this->service.meta_env(this->impl, "CONTENT_LENGTH"); }

    void content_type(const std::string& value)
    { return this->service.meta_env(this->impl, "CONTENT_TYPE"); }

    void gateway_interface(const std::string& value)
    { return this->service.meta_env(this->impl, "GATEWAY_INTERFACE"); }

    void path_info(const std::string& value)
    { return this->service.meta_env(this->impl, "PATH_INFO"); }

    void path_translated(const std::string& value)
    { return this->service.meta_env(this->impl, "PATH_TRANSLATED"); }

    void query_string(const std::string& value)
    { return this->service.meta_env(this->impl, "QUERY_STRING"); }
 
    void remote_addr(const std::string& value)
    { return this->service.meta_env(this->impl, "REMOTE_ADDR"); }

    void remote_host(const std::string& value)
    { return this->service.meta_env(this->impl, "REMOTE_HOST"); }
 
    void remote_ident(const std::string& value)
    { return this->service.meta_env(this->impl, "REMOTE_IDENT"); }
 
    void remote_user(const std::string& value)
    { return this->service.meta_env(this->impl, "REMOTE_USER"); }
 
    void request_method(const std::string& value)
    { return this->service.meta_env(this->impl, "REQUEST_METHOD"); }

    void script_name(const std::string& value)
    { return this->service.meta_env(this->impl, "SCRIPT_NAME"); }

    void server_name(const std::string& value)
    { return this->service.meta_env(this->impl, "SERVER_NAME"); }
 
    void server_port(const std::string& value)
    { return this->service.meta_env(this->impl, "SERVER_PORT"); }
 
    void server_protocol(const std::string& value)
    { return this->service.meta_env(this->impl, "SERVER_PROTOCOL"); }
 
    void server_software(const std::string& value)
    { return this->service.meta_env(this->impl, "SERVER_SOFTWARE"); }

  private:
    char c_;
 
  };

*/
  };

} // namespace cgi

#include "detail/pop_options.hpp"

#endif // CGI_REPLY_HPP_INCLUDED__
