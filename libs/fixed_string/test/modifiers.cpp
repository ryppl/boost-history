// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.5] -- modifiers

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   boost::fixed_string< 15 >           s1;
   boost::fixed_string< 20 >           s2;
   boost::fixed_string< 15 >           s3;

   const char *                        str = "1234567890";

   // assign

   s1.assign( "abc" );
   s2.assign( "123456", 4 );
   s3.assign( 6, '*' );

   BOOST_TEST( s1 == "abc" );
   BOOST_TEST( s2 == "1234" );
   BOOST_TEST( s3 == "******" );

   s1.assign( s2 );
   s3.assign( s2, 1, 2 );

   BOOST_TEST( s1 == s2 );
   BOOST_TEST( s1 == "1234" );
   BOOST_TEST( s3 == "23" );

   s1.assign( str, str + 5 );
   s3.assign( s2.begin(), s2.end());

   BOOST_TEST( s1 == "12345" );
   BOOST_TEST( s3 == "1234" );

   // append

   boost::fixed_string< 15 >           s4( "Hello " );
   boost::fixed_string< 20 >           s5( "abc" );
   boost::fixed_string< 15 >           s6( "1234" );

   s4.append( "World" );
   s5.append( "defghijk", 5 );
   s2.append( 3, '5' );

   BOOST_TEST( s4 == "Hello World" );
   BOOST_TEST( s5 == "abcdefgh" );
   BOOST_TEST( s2 == "1234555" );

   s6.append( s5 );
   s5.append( s6, 2, 4 );
   s1.append( s3.rbegin(), s3.rend());

   BOOST_TEST( s6 == "1234abcdefgh" );
   BOOST_TEST( s5 == "abcdefgh34ab" );
   BOOST_TEST( s1 == "123454321" );

   // push_back

   s1.clear();
   s1.push_back( 'A' );
   s1.push_back( 'B' );
   s1.push_back( 'C' );

   BOOST_TEST( s1 == "ABC" );

   // operator+=

   s1 += s2;
   s2 += "4321";
   s5 += 'c';

   BOOST_TEST( s1 == "ABC1234555" );
   BOOST_TEST( s2 == "12345554321" );
   BOOST_TEST( s5 == "abcdefgh34abc" );

   // insert

   // erase

   s2.erase();
   BOOST_TEST( s2.empty());
   BOOST_TEST( s2 == "" );

   // replace

   // copy

   char                                s[ 20 ];

   s1.copy( s, 5 );
   BOOST_TEST( strcmp( s, "ABC12" ) == 0 );

   s1.copy( s, 5, 3 );
   BOOST_TEST( strcmp( s, "12345" ) == 0 );

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
