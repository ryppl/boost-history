//                  -- scgi/scgi.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_SCGI_SCGI_HPP_INCLUDED__
#define CGI_SCGI_SCGI_HPP_INCLUDED__

#include "boost/cgi/tags.hpp"
#include "boost/cgi/basic_request_fwd.hpp"
#include "boost/cgi/basic_request_acceptor_fwd.hpp"
#include "boost/cgi/basic_protocol_service_fwd.hpp"

namespace cgi {

  /// A class representing the SCGI protocol
  /*
  class scgi
  {
  public:
    // typedefs for common usage
    typedef basic_request<scgi>          request;
    typedef basic_request_acceptor<scgi> acceptor;
    typedef basic_protocol_service<scgi> service;

    typedef boost::asio::ip::tcp         native_protocol;
  };
  */

} // namespace cgi

#endif // CGI_SCGI_SCGI_HPP_INCLUDED__
