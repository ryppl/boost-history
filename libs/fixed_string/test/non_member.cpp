// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.7] -- non-member functions

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 >           s1( "Welt" );
   boost::fixed_string< 15 >           s2( "Welt" );
   boost::fixed_string< 12 >           s3( "World" );

   // lhs + rhs

   boost::fixed_string< 15 >           s4 = "Hello " + s3;
   boost::fixed_string< 15 >           s5 = boost::fixed_string< 15 >( "Hello " ) + s3;

   BOOST_TEST( s4 == s5 );
   BOOST_TEST( s4 == "Hello World" );

   // ==; !=

   BOOST_TEST( s1 == s2 );
   BOOST_TEST( s1 == "Welt" );
   BOOST_TEST( s1 != s3 );
   BOOST_TEST( s1 != "World" );

   // <; <=; >; >=

   BOOST_TEST( s1 < s3 );
   BOOST_TEST( s1 <= s3 );
   BOOST_TEST( s1 <= s2 );
   BOOST_TEST( s3 > s1 );
   BOOST_TEST( s3 >= s1 );
   BOOST_TEST( s2 >= s1 );

   // inserters; extractors; getline

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
