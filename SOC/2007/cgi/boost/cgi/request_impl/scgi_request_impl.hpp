//               -- scgi_request_impl.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_SCGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_SCGI_REQUEST_IMPL_HPP_INCLUDED__

// Make this ProtocolService-independent

namespace cgi {

  /// Implementation for an SCGI request
  class scgi_request_impl
  {
  public:
    typedef tags::scgi    protocol_type;

    scgi_request_impl(protocol_service_type& pserv
                              , const role_type& role = role_type::responder
			      , boost::weak_ptr<connection_base> connection = NULL)
      : protocol_service_(pserv)
      , role_(role)
      , connection_(connection)
      , data_read_(role_ == role_type::filter ? false : true)
      , stdin_read_(role_ == role_type::authorizer ? true : false)
    {
    }

  protected:
    protocol_service_type& protocol_service_;

    /// The role the request plays
    role_type role_;

    map_type env_vars_;
    map_type cookie_vars_;
    map_type post_vars_;
    map_type get_vars_;

    std::string stdin_buffer_;
    std::string data_buffer_; // only needed for request_type::filter (not hugely important)

    /// Finished reading from stdin buffer (ie. POST data)
    bool stdin_read_;

    /// Finished reading from data buffer (for Filter requests)
    bool data_read_;

  private:
    fcgi_request() // private default constructor
    friend class fcgi_service_impl;
  };

} // namespace cgi

#endif // CGI_SCGI_REQUEST_IMPL_HPP_INCLUDED__
