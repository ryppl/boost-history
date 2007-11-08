//              -- detail/scgi_init.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_SCGI_INIT_HPP_INCLUDED__
#define CGI_DETAIL_SCGI_INIT_HPP_INCLUDED__

#include <map>
#include <string>

namespace cgi {
 namespace detail {

  /// Setup the process environment for SCGI use
  /**
   * Not implemented yet. Basically this will catch OS signals
   * and make sure a minor problem doesn't crash the whole server.
   */
  template<typename T>
  class scgi_init
  {
  };

 } // namespace detail
} // namespace cgi

#endif // CGI_DETAIL_SCGI_INIT_HPP_INCLUDED__
