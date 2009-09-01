#ifndef BOOST_CGI_COMMON_MAP_HPP_INCLUDED__
#define BOOST_CGI_COMMON_MAP_HPP_INCLUDED__

#include <map>
#include "boost/cgi/common/name.hpp"

/// You can define your 
#ifndef BOOST_CGI_SESSION_MAP_TYPE
#  define BOOST_CGI_SESSION_MAP_TYPE map
#endif // BOOST_CGI_SESSION_MAP_TYPE

namespace cgi {
 namespace common {

  typedef std::map< ::cgi::common::name, std::string> map;
  typedef std::multimap< ::cgi::common::name, std::string> multimap;

   /**
    * If you want to add a new data type to a request you need to:
    *   > Update this file
    *   > Update source_enums.hpp
    *   > Update `data_map_type` in the `request_base` class
    *   > Use the `BOOST_CGI_DETAIL_MAP_ACCESS` macro in `basic_request<>`,
    *     next to the other uses of it.
    */
  typedef map        env_map;
  typedef map        get_map;
  typedef map        post_map;
  typedef map        form_map;
  typedef map        cookie_map;
  typedef map        session_map;

 } // namespace common
} // namespace cgi

#endif // BOOST_CGI_COMMON_MAP_HPP_INCLUDED__

