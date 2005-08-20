// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FSSAFE_STRING_HPP
#define BOOST_FSSAFE_STRING_HPP
#  include <boost/fixed_string/fixed_string.hpp>
#  include <string.h>

#  if defined(__BORLANDC__) && defined(strcpy) // replace Borland instrinsic string function
#     undef strcpy
      inline char *                              strcpy( char * dest, const char * src )
      {
         return( __strcpy__( dest, src ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strcat) // replace Borland instrinsic string function
#     undef strcat
      inline char *                              strcat( char * dest, const char * src )
      {
         return( __strcat__( dest, src ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strlen) // replace Borland instrinsic string function
#     undef strlen
      inline size_t                              strlen( const char * s )
      {
         return( __strlen__( s ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strcmp) // replace Borland instrinsic string function
#     undef strcmp
      inline int                                 strcmp( const char * s1, const char * s2 )
      {
         return( __strcmp__( s1, s2 ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strncpy) // replace Borland instrinsic string function
#     undef strncpy
      inline char *                              strncpy( char * dest, const char * src, size_t n )
      {
         return( __strncpy__( dest, src, n ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strncat) // replace Borland instrinsic string function
#     undef strncat
      inline char *                              strncat( char * dest, const char * src, size_t n )
      {
         return( __strncat__( dest, src, n ));
      }
#  endif

#  if defined(__BORLANDC__) && defined(strncmp) // replace Borland instrinsic string function
#     undef strncmp
      inline int                                 strncmp( const char * s1, const char * s2, size_t n )
      {
         return( __strncmp__( s1, s2, n ));
      }
#  endif

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
