// (C) Copyright 2003-2004: Reece H. Dunn
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SAFE_STRINGS_HPP
#define BOOST_SAFE_STRINGS_HPP
#  include <boost/fixed_string/char_string.hpp>
#  include <cstdarg>

   // C-string functions

   template< size_t n, class StringTraits >
   inline char *                                 strcpy( boost::char_string< n, char, StringTraits > & d, const char * s )
   {
      return( d.copy( s ));
   }

   template< size_t n, class StringTraits >
   inline size_t                                 strlen( boost::char_string< n, char, StringTraits > & s )
   {
      return( s.length());
   }

   template< size_t n, class StringTraits >
   inline int                                    vsprintf( boost::char_string< n, char, StringTraits > & s, const char * fmt, va_list args )
   {
      return( s.format( fmt, args ));
   }

   template< size_t n, class StringTraits >
   inline int                                    sprintf( boost::char_string< n, char, StringTraits > & s, const char * fmt ... )
   {
      va_list                          args;
      va_start( args, fmt );
      int                              ret = s.format( fmt, args );
      va_end( args );
      return( ret );
   }
#endif
