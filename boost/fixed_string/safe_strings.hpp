// (C) Copyright 2003-2004: Reece H. Dunn
// Use, modification, and distribution are subject to the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SAFE_STRINGS_HPP
#define BOOST_SAFE_STRINGS_HPP
#  include <boost/fixed_string/fixed_string.hpp>
#  include <cstdarg>

   // C-string functions

   inline char *                                 strcpy( boost::char_string & d, const char * s )
   {
      return( d.assign( s ));
   }

   inline char *                                 strcat( boost::char_string & d, const char * s )
   {
      return( d.append( s ));
   }

   inline int                                    strcmp( boost::char_string & d, const char * s )
   {
      return( d.compare( s ));
   }

   inline int                                    strcmp( const char * s, boost::char_string & d )
   {
      return( -d.compare( s ));
   }

   inline size_t                                 strlen( boost::char_string & s )
   {
      return( s.length());
   }

   inline int                                    vsprintf( boost::char_string & s, const char * fmt, va_list args )
   {
      return( s.format( fmt, args ));
   }

   inline int                                    sprintf( boost::char_string & s, const char * fmt ... )
   {
      va_list                          args;
      va_start( args, fmt );
      int                              ret = s.format( fmt, args );
      va_end( args );
      return( ret );
   }
#endif
