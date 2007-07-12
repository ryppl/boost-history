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

namespace cgi {
 namespace detail {

   template<typename Map, typename Separator>
   boost::system::error_code& extract_params(const std::string& input
                                            , Map& destination
                                            , const Separator& separator
                                            , boost::system::error_code& ec)
   {
     if( input.empty() )
       return boost::system::error_code(34, boost::system::errno_ecat);
     
	 typedef boost::tokenizer<Separator>    tokenizer;
     
     tokenizer toker(input, sep);

     std::string name, current_token;

     for(tokenizer::iterator iter = toker.begin()
        ; iter != toker.end()
        ; ++iter)
     {
//       if( *iter == "%" )
//       {
//         current_token += detail::url_decode( *++iter );
//       }else
       if(*iter == "=")
       {
         name = current_token;
         current_token.clear();
       }else
       if( *iter == "&" )
       {
         impl_.get_vars_[name] = current_token;
         current_token.clear();
         name.clear();
       }else
       {
         current_token = url_decode(*iter);
       }
     }
     if( !name.empty() )
       impl_.get_vars_[name] = current_token;
   }

 } // namespace detail
} // namespace cgi

#endif // CGI_DETAIL_EXTRACT_PARAMS_HPP_INCLUDED__
