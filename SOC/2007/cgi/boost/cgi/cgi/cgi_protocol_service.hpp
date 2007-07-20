//             -- cgi_protocol_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_CGI_PROTOCOL_SERVICE_HPP_INCLUDED__
#define CGI_CGI_PROTOCOL_SERVICE_HPP_INCLUDED__

namespace cgi {

  typedef basic_protocol_service<tags::async_cgi> service;

} // namespace cgi

#endif // CGI_CGI_PROTOCOL_SERVICE_HPP_INCLUDED__
