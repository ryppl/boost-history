//                   -- cookie.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_COOKIE_HPP_INCLUDED__
#define CGI_COOKIE_HPP_INCLUDED__

#include <string>

namespace cgi {

  /// A `cookie` object that can be streamed to a `response` object
  /**
   * Either set the parameters in the constructor, or set them directly.
   * Note: If you want to set the parameters individually, remember that each
   * parameter must NOT have a trailing semi-colon!
   *
   * TODO
   * - Add 'domain' attribute
   * - Add 'HttpOnly' attribute
   * - Data should be URL-encoded
   */
  struct cookie
  {
    cookie() {}

    /// Delete the cookie named `_name`.
    cookie(const std::string& _name)
      : content(_name + "=")
      , expires("Fri, 07 May 1824 16:30:00 GMT")
    {
    }

    /// Create a cookie.
    cookie(const std::string& _name, const std::string& _val
          , const std::string& _expires = ""
          , const std::string& _path = "")
      : content(_name + "=" + _val)
      , expires(_expires)
      , path(_path)
    {
    }

    std::string content;
    std::string expires;
    std::string path;
  };

} // namespace cgi

#endif // CGI_COOKIE_HPP_INCLUDED__
