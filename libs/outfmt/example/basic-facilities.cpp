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

   // STL container examples

   std::cout << "vector< int > = "
             << boost::io::formatlist( v )
             << '\n';

   // [results]: vector< int > = [ 41, 18467, 6334, 26500, 19169, 15724, 11478, 29358, 26962, 24464 ]

   std::cout << "list< char > = "
             << boost::io::formatlist( l )
             << '\n';

   // [results]: list< char > = [ A, B, C ]

   // C-style arrays and strings

   std::cout << "int[ 4 ] = "
             << boost::io::formatlist( i, i + 4 )
             << '\n';

   // [results]: int[ 4 ] = [ 3, 6, 9, 12 ]

   std::cout << "char * = "
             << boost::io::formatlist( str, str + ::strlen( str ))
             << '\n';

   // [results]: char * = [ J, e, a, n, e, t, t, e,  , B, i, e, d, e, r, m, a, n, n ]

   // C++-style strings

   std::cout << "std::string = "
             << boost::io::formatlist( s )
             << '\n';

   // [results]: std::string = [ W, a, r, u, m,  , b, i, s, t,  , d, u,  , n, i, c, h, t,  , d, a, ? ]

   return( 0 );
}
