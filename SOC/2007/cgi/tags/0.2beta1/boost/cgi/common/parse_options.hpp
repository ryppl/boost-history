
#ifndef BOOST_CGI_COMMON_PARSE_OPTIONS_HPP_INCLUDED_
#define BOOST_CGI_COMMON_PARSE_OPTIONS_HPP_INCLUDED_

namespace cgi {
 namespace common {

   // Parser options for loading requests. Note that you should *always*
   // parse the environment before using a request object. If you don't parse
   // anything you will have unspecified behaviour.
   enum parse_options
   {
       parse_none      = 0
     , parse_env       = 1
     , parse_get       = 2 | parse_env
     , parse_post      = 4 | parse_env
     , parse_form      = parse_env | parse_get | parse_post
     , parse_cookie    = 8 | parse_env
     , parse_all       = parse_env | parse_form | parse_cookie
   };

 } // namespace common
} // namespace cgi

#endif // BOOST_CGI_COMMON_PARSE_OPTIONS_HPP_INCLUDED_

