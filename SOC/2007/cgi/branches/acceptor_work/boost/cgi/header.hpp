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

  template<typename StringT = std::string>
  struct basic_header
  {
    typedef StringT string_type;
    
    basic_header()
      : content()
    {
    }

    
    /// Templated constructor to allow user-defined types to be converted
    //template<typename T>
    //basic_header(T& t)
    //  : content(t.to_string())
    //{
    //}
    

    //template<>
    basic_header(const string_type& _content)
      : content(_content)
    {
    }

    basic_header(const string_type& name, const string_type& val)
      : content(name + ": " + val)
    {
    }

    //basic_header(const std::string& name, const std::string& val)
    //  : content(name + ": " + val)
    //{
    //}

    /// Construct an header from a cookie.
    template<typename T>
    basic_header(const basic_cookie<T>& ck)
      : content("Set-cookie: " + ck.to_string())
    {
    }

    string_type content;
  };

/*
  template<typename StringT>
  basic_header<StringT>
    make_header(const StringT& name, const StringT& val)
  {
    return basic_header<StringT>(name, val);
  }*/

  template<typename T, typename StringT>
  T make_header(const StringT& name, const StringT& val)
  {
    return basic_header<StringT>(name, val);
  }


  //{ Some shortcuts, to cut down on typing errors.
  template<typename StringT>
  basic_header<StringT>
    content_type(StringT& str)
  {
    return basic_header<StringT>("Content-type", str);
  }

  template<typename StringT>
  basic_header<StringT>
    content_encoding(StringT& str)
  {
    return basic_header<StringT>("Content-encoding", str);
  }
/*
  template<typename StringT>
  basic_header<StringT>
    location(const StringT& url)
  {
    return basic_header<StringT>("Location", url);
  }

  basic_header<std::string>
    location(const std::string& url)
  {
    return basic_header<std::string>("Location", url);
  }*/
  template<typename T>
  T location(const std::string& url)
  {
    return make_header("Location", url);
  }
  //}

  // typedefs for typical usage
  typedef basic_header<std::string>  header;
  typedef basic_header<std::wstring> wheader;

} // namespace cgi

#endif // CGI_HEADER_HPP_INCLUDED__
