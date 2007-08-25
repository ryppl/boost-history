//           -- detail/save_environment.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_SAVE_ENVIRONMENT_HPP_INCLUDED__
#define CGI_DETAIL_SAVE_ENVIRONMENT_HPP_INCLUDED__

#include <string>
#include <cstdlib>
#include "boost/cgi/map.hpp"

namespace cgi {
 namespace detail {

   // The process' environment
   extern char** environ;

   /// Save all information from the process' environment variables to env_map
   void save_environment(cgi::map& env_map)
   {
     for(char** env = ::environ; *env; ++env)
     {
       int i=0;
       int j=strlen(*env);
       for(; i < j; ++i)
         if ((*env)[i] == '=')
           break;

       if ((*env)[i+1] != '\0')
       {
         std::string sa(*env, i);
         std::string sb((*env+i+1), j-i-1);
         env_map[sa] = sb;
       }
     }
   }

 } // namespace detail
} // namespace cgi

#endif // CGI_DETAIL_SAVE_ENVIRONMENT_HPP_INCLUDED__
