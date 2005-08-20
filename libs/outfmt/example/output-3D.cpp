// (C) Copyright 2003: Reece H. Dunn 

#include <iostream>  // std::cout

#include <boost/outfmt/formatlist.hpp>
#include <boost/outfmt/formatob.hpp>

int main()
{
   // 3D array

   char                                a3D[ 3 ][ 3 ][ 3 ] =
   {
      {
         { 'A', '0', '0' },
         { '0', 'A', '0' },
         { '0', '0', 'A' }
      }, {
         { 'B', '0', '0' },
         { '0', 'B', '0' },
         { '0', '0', 'B' }
      }, {
         { 'C', '0', '0' },
         { '0', 'C', '0' },
         { '0', '0', 'C' }
      },
   };

   // formatter configurations

   boost::io::array_output< char * >   arrayfmt;
   arrayfmt.format( "( ", " )" );

   boost::io::array_output< char *, boost::io::array_output< char * > >
                                       array3Dfmt( arrayfmt( 3 ));
   array3Dfmt.format( "{ ", " }", " | " );

   // example using formatter objects

   std::cout << "char[ 3 ][ 3 ][ 3 ] = "
             << boost::io::formatlistout( a3D, a3D + 3, array3Dfmt( 3 ))
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      char[ 3 ][ 3 ][ 3 ] =
      { ( A, 0, 0 ) | ( 0, A, 0 ) | ( 0, 0, A ) }
      { ( B, 0, 0 ) | ( 0, B, 0 ) | ( 0, 0, B ) }
      { ( C, 0, 0 ) | ( 0, C, 0 ) | ( 0, 0, C ) }
   */

   // partially inlined form

   std::cout << "char[ 3 ][ 3 ][ 3 ] = "
             << boost::io::formatlistout
                (
                   a3D, a3D + 3,
                   boost::io::arrayfmtout( arrayfmt( 3 ), 3 ) // inlined form
                   .format( "< ", " >", " : " )
                )
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      char[ 3 ][ 3 ][ 3 ] =
      < ( A, 0, 0 ) : ( 0, A, 0 ) : ( 0, 0, A ) >
      < ( B, 0, 0 ) : ( 0, B, 0 ) : ( 0, 0, B ) >
      < ( C, 0, 0 ) : ( 0, C, 0 ) : ( 0, 0, C ) >
   */

   // fully inlined 3D example
   // [note]: using the generator functions is easier for the more complex constructs
   // as they will deduce the object types for you!

   std::cout << "char[ 3 ][ 3 ][ 3 ] = "
             << boost::io::formatlistout
                (
                   a3D, a3D + 3, // outermost index
                   boost::io::arrayfmtout
                   (
                      boost::io::arrayfmt( 3 ), // innermost index
                      3 // middle index
                   )
                   .format( "< ", " >", " : " )
                )
                .format( "\n", "", "\n" )
            << '\n' << '\n';

   /* [results]:

      char[ 3 ][ 3 ][ 3 ] =
      < [ A, 0, 0 ] : [ 0, A, 0 ] : [ 0, 0, A ] >
      < [ B, 0, 0 ] : [ 0, B, 0 ] : [ 0, 0, B ] >
      < [ C, 0, 0 ] : [ 0, C, 0 ] : [ 0, 0, C ] >
   */

   // more complex inlined example
   // [note]: using the generator functions is easier for the more complex constructs
   // as they will deduce the object types for you!

   std::cout << "char[ 3 ][ 3 ][ 3 ] = "
             << boost::io::formatlistout
                (
                   a3D, a3D + 3, // outermost index
                   boost::io::arrayfmtout
                   (
                      boost::io::arrayfmtex< char * >( 3 ) // innermost index
                      .format( "( ", " )" ), // [note]: cannot use characters here; must be char *.
                      3 // middle index
                   )
                   .format( "{\n   ", "\n}", "\n   " )
                )
                .format( "\n", "" )
             << '\n' << '\n';

   /* [results]:

      char[ 3 ][ 3 ][ 3 ] =
      {
         ( A, 0, 0 )
         ( 0, A, 0 )
         ( 0, 0, A )
      }, {
         ( B, 0, 0 )
         ( 0, B, 0 )
         ( 0, 0, B )
      }, {
         ( C, 0, 0 )
         ( 0, C, 0 )
         ( 0, 0, C )
      }
   */

#  if !defined(BOOST_IOFM__NO_OUTPUT_DEDUCTION) && !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
      std::cout << "deduced output: " << boost::io::formatob( a3D ) << '\n';

      // [results:] deduced output: [ [ [ A, 0, 0 ], [ 0, A, 0 ], [ 0, 0, A ] ],
      //    [ [ B, 0, 0 ], [ 0, B, 0 ], [ 0, 0, B ] ], [ [ C, 0, 0 ], [ 0, C, 0 ],
      //    [ 0, 0, C ] ] ]
#  endif

   return( 0 );
}
