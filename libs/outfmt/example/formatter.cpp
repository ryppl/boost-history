// (C) Copyright 2003: Reece H. Dunn

#include <iostream>  // std::cout
#include <string>    // std::string
#include <algorithm> // std::generate
#include <vector>    // std::vector
#include <list>      // std::list
#include <stdlib.h>  // rand
#include <string.h>  // strlen

#include <boost/outfmt/formatlist.hpp>

int main()
{
   // std::vector

   std::vector< int >                  v( 10 );
   std::generate( v.begin(), v.end(), ::rand );

   // std::list

   std::list< char >                   l;
   l.push_back( 'A' );
   l.push_back( 'B' );
   l.push_back( 'C' );

   // std::string and arrays

   const char                        * str  = "Jeanette Biedermann";
   std::string                         s    = "Warum bist du nicht da?";

   int                                 i[ 4 ] = { 3, 6, 9, 12 };

   // formatter configurations

   boost::io::openclose_formatter< char * >
                                       ocf(  "| ", " |" );
   boost::io::formatter< char * >      fmt(  "( ", " )", ", " );
   boost::io::formatter< char * >      fmt2( " -- " );

   // examples

   std::cout << "int[ 4 ] = "
             << boost::io::formatlist( i, i + 4 ).format( ocf )
             << '\n';

   // [results]: int[ 4 ] = | 3, 6, 9, 12 |

   std::cout << "int[ 4 ] = "
             << boost::io::formatlist( i, i + 4 ).format( fmt )
             << '\n';

   // [results]: int[ 4 ] = ( 3, 6, 9, 12 )

   std::cout << "char * = "
             << boost::io::formatlist( str, str + ::strlen( str )).format( fmt )
             << '\n';

   // [results]: char * = ( J, e, a, n, e, t, t, e,  , B, i, e, d, e, r, m, a, n, n )

   std::cout << "list< char > = "
             << boost::io::formatlist( l ).format( fmt )
             << '\n';

   // [results]: int[ 4 ] = ( A, B, C )

   std::cout << "list< char > = "
             << boost::io::formatlist( l ).format( fmt2 )
             << '\n';

   // [results]: int[ 4 ] = ( A -- B -- C )

   return( 0 );
}
