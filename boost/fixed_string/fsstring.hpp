// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FSSAFE_STRING_HPP
#define BOOST_FSSAFE_STRING_HPP
#  include <boost/fixed_string/fixed_string.hpp>
#  include <string.h>

   // string.h functions

   inline char *                                 strcpy( boost::char_string & d, const char * s )
   {
      d.assign( s );
      return( d.buffer());
   }

   inline char *                                 strcat( boost::char_string & d, const char * s )
   {
      d.append( s );
      return( d.buffer());
   }

   inline int                                    strcmp( boost::char_string & s1, boost::char_string & s2 )
   {
      return( s1.compare( s2 ));
   }

   inline int                                    strcmp( boost::char_string & s1, const char * s2 )
   {
      return( s1.compare( s2 ));
   }

   inline int                                    strcmp( const char * s1, boost::char_string & s2 )
   {
      return( -s2.compare( s1 ));
   }

   inline size_t                                 strlen( boost::char_string & s )
   {
      return( s.length());
   }

   inline char *                                 strncpy( boost::char_string & d, const char * s, size_t n )
   {
      d.assign( s, n );
      return( d.buffer());
   }

   inline char *                                 strncat( boost::char_string & d, const char * s, size_t n )
   {
      d.append( s, n );
      return( d.buffer());
   }

   inline int                                    strncmp( boost::char_string & s1, boost::char_string & s2, size_t n )
   {
      return( s1.compare( 0, s1.size(), s2, 0, n ));
   }

   inline int                                    strncmp( boost::char_string & s1, const char * s2, size_t n )
   {
      return( s1.compare( 0, s1.size(), s2, n ));
   }

   inline int                                    strncmp( const char * s1, boost::char_string & s2, size_t n )
   {
      return( -s2.compare( 0, s2.size(), s1, n ));
   }
#endif
