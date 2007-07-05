//               -- fcgi_request_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_FCGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_FCGI_REQUEST_IMPL_HPP_INCLUDED__

namespace cgi {

  /// Implementation for a FastCGI Responder request
  class fcgi_responder_request_impl
  {
    fcgi_request() {} // private default constructor
  public:
    fcgi_request(protocol_service_type& pserv)
      : protocol_service_(pserv)
    {
    }

    bool load(bool parse_stdin)
    {
    }

    template<typename Handler>
    void async_load(bool parse_stdin, Handler handler)
    {
    }

    /// The role of the request; in this case a responder
    static role_type& role() { return role_type::responder; }
  private:
    protocol_service_type& protocol_service_;

    /// Finished reading from stdin buffer (ie. POST data)
    bool stdin_read_;
  };

} // namespace cgi

#endif // CGI_FCGI_REQUEST_IMPL_HPP_INCLUDED__
