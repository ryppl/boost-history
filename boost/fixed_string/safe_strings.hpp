// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_SAFE_STRINGS_HPP
#define BOOST_SAFE_STRINGS_HPP
#  include <boost/char_string.hpp>
#  include <cstdarg>

   namespace boost
   {
      template< size_t n, bool nt, typename CharT, class StringTraits >
      inline CharT *                             cstrcpy( char_string< n, nt, CharT, StringTraits > & d, const CharT * s )
      {
         return( d.copy( s ));
      }
      template< size_t n, bool nt, typename CharT, class StringTraits >
      inline size_t                              cstrlen( char_string< n, nt, CharT, StringTraits > & s )
      {
         return( s.length());
      }
      template< size_t n, bool nt, typename CharT, class StringTraits >
      inline int                                 vcsprintf( char_string< n, nt, CharT, StringTraits > & s, const CharT * fmt, va_list args )
      {
         return( s.format( fmt, args ));
      }
      template< size_t n, bool nt, typename CharT, class StringTraits >
      inline int                                 csprintf( char_string< n, nt, CharT, StringTraits > & s, const CharT * fmt ... )
      {
         va_list                       args;
         va_start( args, fmt );
         int                           ret = s.format( fmt, args );
         va_end( args );
         return( ret );
      }
   }
#endif
