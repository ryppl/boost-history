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

#include <map>
#include <string>
#include <cstdlib>
#include "boost/cgi/map.hpp"

// The process' environment
extern char** environ;

namespace cgi {
 namespace detail {

   /// Save all information from `environment` to env_map
   /**
    * @param env This defaults to `::environ`, or the current process'
    *            environment.
    */
   void save_environment(std::map<std::string,std::string>& env_map
                        , char** env = ::environ)
   {
     for(; *env; ++env)
     {
       int i=0;
       int j=strlen(*env);
       for(; i < j; ++i)
         if ((*env)[i] == '=')
           break;

       // Note: empty variables are not guaranteed to be set by the server, so
       // we are free to ignore them too.
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

/* Alternative version which doesn't copy the 'value' of the variable
 ******************************************************************************
   void save_environment(std::map<std::string,const char*>& env_map
                        , char** env = ::environ)
   {
     for(; *env; ++env)
     {
       int i=0;
       int j=strlen(*env);
       for(; i < j; ++i)
         if ((*env)[i] == '=')
           break;

       // Note: empty variables are not guaranteed to be set by the server, so
       // we are free to ignore them too.
       if ((*env)[i+1] != '\0')
       {
         std::string sa(*env, i);
         env_map[sa] = (*env+i+1);
       }
     }
   }
 ******************************************************************************
 */

#endif // CGI_DETAIL_SAVE_ENVIRONMENT_HPP_INCLUDED__
