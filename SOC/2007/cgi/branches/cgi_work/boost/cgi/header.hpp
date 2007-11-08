//                   -- header.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_HEADER_HPP_INCLUDED__
#define CGI_HEADER_HPP_INCLUDED__

#include <string>

namespace cgi {

  struct header
  {
    header()
      : content()
    {
    }

    /*
    /// Templated constructor to allow user-defined types to be converted
    template<typename T>
    header(T& t)
      : content(t.to_string())
    {
    }
    */

    //template<>
    header(const std::string& _content)
      : content(_content)
    {
    }

    header(const std::string& name, const std::string& val)
      : content(name + ": " + val)
    {
    }

    std::string content;
  };

  // Some shortcuts, to cut down on errors
  header content_type(const std::string& str)
  {
    return header("Content-type", str);
  }

  header location(const std::string& url)
  {
    return header("Location", url);
  }

} // namespace cgi

#endif // CGI_HEADER_HPP_INCLUDED__
