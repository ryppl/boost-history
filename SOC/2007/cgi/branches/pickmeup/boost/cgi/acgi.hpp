//                   -- acgi.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_ACGI_HPP_INCLUDED__
#define CGI_ACGI_HPP_INCLUDED__

#include "boost/cgi/acgi/service.hpp"
#include "boost/cgi/acgi/request.hpp"
#include "boost/cgi/detail/common_headers.hpp"

BOOST_CGI_NAMESPACE_BEGIN
 namespace acgi {

   using namespace ::BOOST_CGI_NAMESPACE::common; // import common namespace elements.

 } // namespace acgi
BOOST_CGI_NAMESPACE_END

/// Dump acgi stuff into the boost namespace
namespace boost {
 namespace acgi {
   using namespace ::BOOST_CGI_NAMESPACE::acgi;
 } // namespace acgi
} // namespace boost

#endif // CGI_ACGI_HPP_INCLUDED__
