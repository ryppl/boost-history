// (C) Copyright 2003: Reece H. Dunn 

#include <iostream>  // std::cout
#include <list>      // std::list

#include <boost/outfmt/formatlist.hpp>
#include <boost/outfmt/formatob.hpp>

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

   boost::io::array_output< char * >   arrayfmt;
   arrayfmt.format( "( ", " )" );

   boost::io::container_output< char * > containerfmt;
   containerfmt.format( "| ", " |", " | " );

   // examples

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatlistout( a2D, a2D + 3, arrayfmt( 3 ))
             << '\n';

   // [results:] int[ 3 ][ 3 ] = [ ( 1, 0, 0 ), ( 0, 1, 0 ), ( 0, 0, 1 ) ]

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatlistout
                (
                   a2D, a2D + 3,
                   boost::io::arrayfmt( 3 ).format( "( ", " )" ) // inlined form
                )
             << '\n' << '\n';

   // [results]: int[ 3 ][ 3 ] = [ ( 1, 0, 0 ), ( 0, 1, 0 ), ( 0, 0, 1 ) ]

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatlistout( a2D, a2D + 3, arrayfmt( 3 ))
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      int[ 3 ][ 3 ] =
      ( 1, 0, 0 )
      ( 0, 1, 0 )
      ( 0, 0, 1 )
   */

   std::cout << "int[ 3 ][ 3 ] = "
             << boost::io::formatlistout( a2D, a2D + 3, arrayfmt( 1, 2 ))
               .format( "\n", "\0", "\n" )
             << '\n' << '\n';

   /* [results]:

      int[ 3 ][ 3 ] =
      ( 0, 0 )
      ( 1, 0 )
      ( 0, 1 )
   */

   std::cout << "std::list< std::list< char > > tictactoe = "
             << boost::io::formatlistout( tictactoe, containerfmt )
                .format( "\n", "", "\n" )
             << '\n' << '\n';

   /* [results]:

      std::list< std::list< char > > tictactoe =
      | O | X | O |
      | X | O | X |
      | O | X | O |
   */

   std::cout << "std::list< std::list< char > > tictactoe = "
             << boost::io::formatlistout
                (
                   tictactoe,
                   boost::io::containerfmt() // inlined form
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

#  if !defined(BOOST_IOFM__NO_OUTPUT_DEDUCTION)
      std::cout << "deduced output: " << boost::io::formatob( tictactoe ) << '\n'; 

      // [results:] deduced output: [ [ O, X, O ], [ X, O, X ], [ O, X, O ] ]

#     if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
         std::cout << "deduced output: " << boost::io::formatob( a2D ) << '\n';

         // [results:] deduced output: [ [ 1, 0, 0 ], [ 0, 1, 0 ], [ 0, 0, 1 ] ]
#     endif
#  endif

   return( 0 );
}
