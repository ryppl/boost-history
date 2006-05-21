// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>  // std::cout
#include <list>      // std::list

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/stl/list.hpp>

int main()
{
   // 2D array

   int                                 a2D[ 3 ][ 3 ] =
   {
      { 1, 0, 0 },
      { 0, 1, 0 },
      { 0, 0, 1 }
   };

   // 2D container

   std::list< std::list< char > >      tictactoe;

   tictactoe.push_back( std::list< char >());
      tictactoe.back().push_back( 'O' );
      tictactoe.back().push_back( 'X' );
      tictactoe.back().push_back( 'O' );
   tictactoe.push_back( std::list< char >());
      tictactoe.back().push_back( 'X' );
      tictactoe.back().push_back( 'O' );
      tictactoe.back().push_back( 'X' );
   tictactoe.push_back( std::list< char >());
      tictactoe.back().push_back( 'O' );
      tictactoe.back().push_back( 'X' );
      tictactoe.back().push_back( 'O' );

   // formatter configurations

   boost::io::arrayfmt_t< const char * > arrayfmt;
   arrayfmt.format( "( ", " )" );

   boost::io::containerfmt_t< const char * > containerfmt;
   containerfmt.format( "| ", " |", " | " );

   // examples

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatob
                (
                   boost::io::range( a2D, a2D + 3 ),
                   boost::io::rangefmt( arrayfmt( 3 ))
                )
             << '\n';

   // [results:] int[ 3 ][ 3 ] = [ ( 1, 0, 0 ), ( 0, 1, 0 ), ( 0, 0, 1 ) ]

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatob
                (
                   boost::io::range( a2D, a2D + 3 ),
                   boost::io::rangefmt( boost::io::arrayfmt( 3 ).format( "( ", " )" )) // inlined form
                )
             << '\n' << '\n';

   // [results]: int[ 3 ][ 3 ] = [ ( 1, 0, 0 ), ( 0, 1, 0 ), ( 0, 0, 1 ) ]

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatob
                (
                   boost::io::range( a2D, a2D + 3 ),
                   boost::io::rangefmt( arrayfmt( 3 ))
                )
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      int[ 3 ][ 3 ] =
      ( 1, 0, 0 )
      ( 0, 1, 0 )
      ( 0, 0, 1 )
   */

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatob
                (
                   boost::io::range( a2D, a2D + 3 ),
                   boost::io::rangefmt( arrayfmt( 1, 2 ))
                )
               .format( "\n", "\0", "\n" )
             << '\n' << '\n';

   /* [results]:

      int[ 3 ][ 3 ] =
      ( 0, 0 )
      ( 1, 0 )
      ( 0, 1 )
   */

   std::cout << "std::list< std::list< char > > tictactoe = "
             << boost::io::formatob( tictactoe, boost::io::containerfmt( containerfmt ))
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      std::list< std::list< char > > tictactoe =
      | O | X | O |
      | X | O | X |
      | O | X | O |
   */

   std::cout << "std::list< std::list< char > > tictactoe = "
             << boost::io::formatob
                (
                   tictactoe,
                   boost::io::containerfmt( boost::io::containerfmt()) // inlined form
                   .format( " | " )
                )
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /*
      std::list< std::list< char > > tictactoe =
      [ O | X | O ]
      [ X | O | X ]
      [ O | X | O ]
   */

   std::cout << "deduced output: " << boost::io::formatob( tictactoe ) << '\n';

   // [results:] deduced output: [ [ O, X, O ], [ X, O, X ], [ O, X, O ] ]

#  if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
      std::cout << "deduced output: " << boost::io::formatob( a2D ) << '\n';

      // [results:] deduced output: [ [ 1, 0, 0 ], [ 0, 1, 0 ], [ 0, 0, 1 ] ]
#  endif

   return( 0 );
}
