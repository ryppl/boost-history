//                  -- cgi_request.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_REQUEST_HPP_INCLUDED__
#define CGI_CGI_REQUEST_HPP_INCLUDED__

#include "request_service.hpp"
#include "../tags.hpp"
#include "../basic_request_fwd.hpp"
#include "../request_impl/cgi_request_impl.hpp"
#include "../basic_request.hpp"

namespace cgi {

  class cgi_service_impl;

  typedef basic_request<cgi_request_service, cgi_service> cgi_request;

} // namespace cgi

#endif // CGI_CGI_REQUEST_HPP_INCLUDED__
