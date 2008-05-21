//               -- common/io_service.hpp --
//
//            Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_COMMON_IO_SERVICE_HPP_INCLUDED__
#define CGI_COMMON_IO_SERVICE_HPP_INCLUDED__

//#if _MSC_VER > 1020
//#pragma once
//#endif

#include <boost/asio/io_service.hpp>

namespace cgi {
 namespace common {

  using boost::asio::io_service;

 } // namespace common

  // This should go?
  using boost::asio::io_service;

} // namespace cgi

#endif // CGI_COMMON_IO_SERVICE_HPP_INCLUDED__

