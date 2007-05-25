//
// request.hpp
// -----------
//
// Copyright (c) 2007 Darren Garvey
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __CGI_REQUEST_HPP_INCLUDE_
#define __CGI_REQUEST_HPP_INCLUDE_

#include <boost/noncopyable.hpp>
#include "request_base.hpp"
#include "basic_cgi_service_fwd.hpp"
#include "basic_cgi_request_fwd.hpp"

namespace cgi {

/// The 'request' class that is the user's main entry point into this library
/**
 * By default the class will work with 'any' service type to provide uniform
 * access to requests handled by any cgi::service type. If no service is
 * provided to the constructor, the object is assumed to be a standard cgi
 * object.
 *
 * @par Example
 * @code
 * void f()
 * {
 *   cgi::request r; // standard cgi object
 * }
 *
 * void g()
 * {
 *   cgi::fcgi_service service; // fastcgi service
 *   cgi::request(service); // fastcgi request
 * }
 * @endcode
 *
 * If runtime linkage is insufficient, a macro can be defined to instead use 
 * compile-time linking. Available macros:
 *
 * @li @code BOOST_CGI_IMPLICIT_CGI @endcode -> standard cgi
 * @li @code BOOST_CGI_IMPLICIT_FCGI @endcode -> fastcgi
 *
 * {{can modern compilers optimize away the runtime element of the general
 * version of request?}}
 */

#if defined(BOOST_CGI_IMPLICIT_CGI)
  typedef basic_cgi_request<protocol::cgi> request;
#elif defined(BOOST_CGI_IMPLICIT_FCGI)
  typedef basic_cgi_request<protocol::fcgi> request;
#elif defined(BOOST_CGI_IMPLICIT_SCGI)
# error "SCGI driver not implemented yet"
#else

  // Below is the 'general' request class
  class request
    : private boost::noncopyable
  {
      boost::shared_ptr<request_base> impl_;
  public:
    template< typename Protocol, typename Allocator = std::allocator() >
    explicit request(basic_cgi_service<Protocol, Allocator>& service)
      : impl_(&service.get_request())
    {
    }

    template< typename ServiceType, typename Allocator = std::allocator() >
    request(basic_cgi_request<ServiceType, Allocator>& request)
      : impl_(&request)
    {
    }

    template< typename Allocator = std::allocator() >
    request()
      : impl_(new basic_cgi_request< basic_cgi_service<protocol::cgi, Allocator>
                                   , Allocator
                                   > request
             )
    {
    }

    void end( int return_code )
    {
        impl_->end(return_code);
    }

    // more functions
  };

} // namespace cgi

#endif // __CGI_REQUEST_HPP_INCLUDE_

