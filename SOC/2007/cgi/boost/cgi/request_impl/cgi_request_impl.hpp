//              -- cgi_request_impl.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__
#define CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__

#include <map>
#include <string>
#include <boost/noncopyable.hpp>

#include "http/status_code.hpp"
#include

// Make this ProtocolService-independent

namespace cgi {

  /// Implementation for a standard CGI request
  /**
   * Note: This isn't noncopyable since there's no real reason it can't be
   * copied around. Since basic_request is noncopyable, basic copying will be
   * restricted but if someone really wants to copy the data, then they can.
   */
  class cgi_request_impl
  {
  public:
    typedef std::map<std::string, std::string>    map_type;

    /// Constructor
    /**
     * Since this request type is synchronous, there is no need for an
     * io_service, so the passed ProtocolService is just ignored.
     */
    template<typename ProtocolService>
    cgi_request_impl(ProtocolService& pserv)
      : stdin_read_(false)
      , http_status_(http::status_code::ok)
      , request_status_(request_status::unloaded)
    {
    }

  private:
    friend class cgi_service_impl;

  protected:
    map_type get_vars_;
    map_type post_vars_;
    map_type cookie_vars_;

    bool stdin_read_;

    http::status_code http_status_;
    request_status request_status_;
  };

  template<> inline const std::string&
  cgi_request_impl::var<tags::ENV>(const std::string& name)
  {
    return ::getenv(name);
  }

  /// Get a request map of all the environment meta-variables (slow)
  /**
   * -- NOT IMPLEMENTED FOR NOW --
   *
   * In the case of a CGI request, the environment meta-data is usually stored
   * in the process environment, which means there is no direct access to all
   * of them as a map_type&. In other words, this function call will have to
   * load all of the variables into memory and then return the map
   */
  //template<> inline cgi_request_impl::map_type&
  //cgi_request_impl::var<tags::ENV>()
  //{
  //  throw std::logic_error("Can't get all environment vars as a map_type&");
  //}

  //template<> inline cgi_request_impl::map_type&
  //cgi_request_impl::var<tags::HTTP>() { return http_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::COOKIE>() { return cookie_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::GET>() { return get_map_; }

  template<> inline cgi_request_impl::map_type&
  cgi_request_impl::var<tags::POST>() { return post_map_; }

} // namespace cgi

#endif // CGI_CGI_REQUEST_IMPL_HPP_INCLUDED__
