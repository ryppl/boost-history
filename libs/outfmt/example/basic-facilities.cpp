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
   std::generate( v.begin(), v.end(), ::rand ); // randomly fill the vector

   // std::list

   std::list< char >                   l;
   l.push_back( 'A' );
   l.push_back( 'B' );
   l.push_back( 'C' );

   // std::string and C-style arrays/strings

   const char                        * str  = "Jeanette Biedermann";
   std::string                         s    = "Warum bist du nicht da?";

   int                                 i[ 4 ] = { 3, 6, 9, 12 };
   std::pair< int *, int * >           ip = std::pair< int *, int * >( i, i + 2 );

   // STL container examples

   std::cout << "vector< int > = "
             << boost::io::formatob( v )
             << '\n';

   // [results]: vector< int > = [ 41, 18467, 6334, 26500, 19169, 15724, 11478, 29358, 26962, 24464 ]

   std::cout << "list< char > = "
             << boost::io::formatob( boost::io::range( l ))
             << '\n';

   // [results]: list< char > = [ A, B, C ]

   // C-style arrays and strings

   std::cout << "int[ 4 ] = "
             << boost::io::formatob( boost::io::range( i, i + 4 ))
             << '\n';

   // [results]: int[ 4 ] = [ 3, 6, 9, 12 ]

   std::cout << "char * = "
             << boost::io::formatob( boost::io::range( str, str + ::strlen( str )))
             << '\n';

   // [results]: char * = [ J, e, a, n, e, t, t, e,  , B, i, e, d, e, r, m, a, n, n ]

   // C++-style strings

   std::cout << "std::string = "
             << boost::io::formatob( s )
             << '\n';

   // [results]: std::string = Warum bist du nicht da?

   // Iterator ranges in a std::pair

   std::cout << "std::pair< int *, int * > = "
             << boost::io::formatob( boost::io::range( ip ))
             << '\n';

   // [results]: int[ 4 ] = [ 3, 6 ]

   return( 0 );
}
