// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.5] -- modifiers

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   // operator+=
   // append
   // push_back
   // assign
   // insert
   // erase
   // replace
   // copy

   // swap

   {
      boost::fixed_string< 15 >        s1( "abcdef" );
      boost::fixed_string< 30 >        s2( "12345" );
      s1.swap( s2 );

      BOOST_TEST( s1 == "12345" );
      BOOST_TEST( s2 == "abcdef" );
   }

   return( 0 );
}
