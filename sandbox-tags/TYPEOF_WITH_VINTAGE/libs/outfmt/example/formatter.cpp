// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>  // std::cout
#include <string>    // std::string
#include <algorithm> // std::generate
#include <stdlib.h>  // rand
#include <string.h>  // strlen

#include <boost/outfmt/formatob.hpp>
#include <boost/outfmt/stl/vector.hpp>
#include <boost/outfmt/stl/list.hpp>

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

   boost::io::openclose_formatter< const char * >
                                        ocf(  "| ", " |" );
   boost::io::formatter< const char * > fmt(  "( ", " )", ", " );
   boost::io::formatter< const char * > fmt2( " -- " );

   // examples

   std::cout << "int[ 4 ] = "
             << boost::io::formatob( boost::io::range( i, i + 4 )).format( ocf )
             << '\n';

   // [results]: int[ 4 ] = | 3, 6, 9, 12 |

   std::cout << "int[ 4 ] = "
             << boost::io::formatob( boost::io::range( i, i + 4 )).format( fmt )
             << '\n';

   // [results]: int[ 4 ] = ( 3, 6, 9, 12 )

   std::cout << "char * = "
             << boost::io::formatob( boost::io::range( str, str + ::strlen( str )))
                .format( fmt )
             << '\n';

   // [results]: char * = ( J, e, a, n, e, t, t, e,  , B, i, e, d, e, r, m, a, n, n )

   std::cout << "list< char > = "
             << boost::io::formatob( l ).format( fmt )
             << '\n';

   // [results]: int[ 4 ] = ( A, B, C )

   std::cout << "list< char > = "
             << boost::io::formatob( l ).format( fmt2 )
             << '\n';

   // [results]: int[ 4 ] = [ A -- B -- C ]

   return( 0 );
}
