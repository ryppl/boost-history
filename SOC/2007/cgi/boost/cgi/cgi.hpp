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
#include "cgi/cgi_request.hpp"
#include "cgi/cgi_service.hpp"
#include "service_impl/cgi_service_impl.hpp"
#include "request_impl/cgi_request_impl.hpp"
#include "gateway_impl/cgi_gateway_impl.hpp"
#include "gateway_service/cgi_gateway_service.hpp"

// Include headers common to all protocols
#include "detail/common_headers.hpp"

#endif // CGI_CGI_HPP_INCLUDED__
