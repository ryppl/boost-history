//                -- extract_params.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_DETAIL_EXTRACT_PARAMS_HPP_INCLUDED__
#define CGI_DETAIL_EXTRACT_PARAMS_HPP_INCLUDED__

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/system/error_code.hpp>

#include "boost/cgi/detail/url_decode.hpp"

BOOST_CGI_NAMESPACE_BEGIN
 namespace detail {

   /// Extract name/value pairs from a supplied string buffer.
   /**
    * Name/value pairs can be separated by either '&' or ';', and the
    * format is: name=value
    *
    */
   template<typename String, typename Map, typename Separator>
   boost::system::error_code&
   extract_params(const String& input
                 , Map& destination
                 , const Separator& separator
                 , boost::system::error_code& ec)
   {
     if( input.empty() )
       return ec;// = boost::system::error_code(34, boost::system::errno_ecat);

     typedef typename boost::tokenizer<Separator>    tokenizer;
     typedef typename Map::value_type                value_type;

     tokenizer toker(input, separator);

     std::string name, current_token;

     for(typename tokenizer::iterator iter = toker.begin()
        ; iter != toker.end()
        ; ++iter)
     {
       if(*iter == "=")
       {
         name = current_token;
         current_token.clear();
       }else
       if( *iter == "&" || *iter == ";" )
       {
// Empty parameters (eg. `empty` in /path/to/script?empty&foo=bar) aren't
// guaranteed by the CGI spec to be kept, but you might want to use them.
// You just have to define `BOOST_CGI_KEEP_EMPTY_VARS` (**TODO** currently
// on by default).
// Note that you'll want to check that your server keeps empty query string 
// parameters.
#if defined(BOOST_CGI_KEEP_EMPTY_VARS)
         if (name.empty())
           destination.insert(value_type(current_token.c_str(), ""));
         else
#endif // BOOST_CGI_KEEP_EMPTY_VARS
           destination.insert(value_type(name.c_str(), current_token));
         current_token.clear();
         name.clear();
       }else
       {
         current_token = url_decode(*iter);
       }
     }
     // Save the name if the last n/v pair has no value.
     if ( !name.empty() )
       destination.insert(value_type(name.c_str(), current_token));
#if defined(BOOST_CGI_KEEP_EMPTY_VARS)
     else // Save the final 'toggle' - eg /path/to/script?foo=bar&toggle
     if ( !current_token.empty() )
       destination.insert(value_type(current_token.c_str(), ""));
#endif

     return ec;
   }

 } // namespace detail
BOOST_CGI_NAMESPACE_END

#endif // CGI_DETAIL_EXTRACT_PARAMS_HPP_INCLUDED__
