// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/stl/list.hpp>   // std::list support

#include "test.hpp"

int test_main( int, char *[] )
{
   std::stringstream                   ss;

   std::list< char >                   lw;
   lw.push_back( 'A' );
   lw.push_back( 'B' );
   lw.push_back( 'C' );

   boost::io::openclose_formatter< const char * >
                                       ocfmt( "< ", " >" );        // < a, b, ... >
   boost::io::formatter< const char * >  fmt( "< ", " >", " | " ); // < a | b | ... >

   // boost::io::formatter_t< ... >::format

   {
      const char *                     res = "[ A : B : C ]";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format( " : " ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format( " : " ), res );
   }
   {
      const char *                     res = "{ A, B, C }";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format( "{ ", " }" ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format( "{ ", " }" ), res );
   }
   {
      const char *                     res = "{ A : B : C }";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format( "{ ", " }", " : " ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format( "{ ", " }", " : " ), res );
   }

   // boost::io::openclose_formatter< ... >

   {
      const char *                     res = "[ A, B, C ]";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::openclose_formatter< const char * >()
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::openclose_formatter< const char * >()
                   ), res );
   }
   {
      const char *                     res = "{ A, B, C }";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::openclose_formatter< const char * >( "{ ", " }" )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::openclose_formatter< const char * >( "{ ", " }" )
                   ), res );
   }
   {
      const char *                     res = "< A, B, C >";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::openclose_formatter< const char * >( ocfmt )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::openclose_formatter< const char * >( ocfmt )
                   ), res );
   }

   // boost::io::formatter< ... >

   {
      const char *                     res = "[ A, B, C ]";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >()
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >()
                   ), res );
   }
   {
      const char *                     res = "[ A : B : C ]";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( " : " )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( " : " )
                   ), res );
   }
   {
      const char *                     res = "{ A, B, C }";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( "{ ", " }" )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( "{ ", " }" )
                   ), res );
   }
   {
      const char *                     res = "{ A : B : C }";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( "{ ", " }", " : " )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( "{ ", " }", " : " )
                   ), res );
   }
   {
      const char *                     res = "< A, B, C >";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( ocfmt )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( ocfmt )
                   ), res );
   }
   {
      const char *                     res = "< A : B : C >";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( ocfmt, " : " )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( ocfmt, " : " )
                   ), res );
   }
   {
      const char *                     res = "< A | B | C >";
      std::list< char >                lr;
      IOFMT_READ(  boost::io::formatob( lr ).format
                   (
                      boost::io::formatter< const char * >( fmt )
                   ), lr, lw, res );
      IOFMT_WRITE( boost::io::formatob( lw ).format
                   (
                      boost::io::formatter< const char * >( fmt )
                   ), res );
   }

   return( 0 );
}
