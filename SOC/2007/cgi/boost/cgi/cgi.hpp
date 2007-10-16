//                   -- cgi/cgi.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_HPP_INCLUDED__
#define CGI_CGI_HPP_INCLUDED__

// Include all cgi-related headers only.
#include "boost/cgi/request.hpp"
#include "boost/cgi/service.hpp"

// Include headers common to all protocols
#include "boost/cgi/detail/common_headers.hpp"

namespace cgi {

  typedef cgi_request request;
  //typedef cgi::cgi_service service; // to be added
  
} // namespace cgi

#endif // CGI_CGI_HPP_INCLUDED__
