//                  -- take_buffer.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
#ifndef CGI_TAKE_BUFFER_HPP_INCLUDED__
#define CGI_TAKE_BUFFER_HPP_INCLUDED__

namespace cgi {
 namespace detail {

   /// Take a buffer from t
   template<typename T>
   std::streambuf* take_buffer(T& t)
   {}

 } // namespace detail
} // namespace cgi

#endif // CGI_TAKE_BUFFER_HPP_INCLUDED__
