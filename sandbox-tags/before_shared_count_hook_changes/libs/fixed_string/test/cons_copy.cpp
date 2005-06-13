// (C) Copyright 2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.1] -- construct/copy/destroy

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   typedef boost::fixed_string< 15 >   string1;
   typedef boost::fixed_string< 25 >   string2;

   string1                             s1;
   BOOST_TEST( s1 == "" );
   BOOST_TEST( s1.length() == 0 );

   // construction from primitive types

   string1                             s2( "abcdef" );
   string2                             s3( "abcdef", 3 );
   string2                             s4( 5, '0' );

   BOOST_TEST( s2 == "abcdef" );
   BOOST_TEST( s3 == "abc" );
   BOOST_TEST( s4 == "00000" );

   // construction from fixed_string

   string1                             s5( s2 );
   string1                             s6( s2, 3 );
   string1                             s7( s2, 2, 3 );

   BOOST_TEST( s2 == s5 );
   BOOST_TEST( s5 == "abcdef" );
   BOOST_TEST( s6 == "def" );
   BOOST_TEST( s7 == "cde" );

   // assignment

   s1 = "1234567890";
   s2 = 'a';
   s3 = s1;

   BOOST_TEST( s1 == "1234567890" );
   BOOST_TEST( s2 == "a" );
   BOOST_TEST( s3 == "1234567890" );

   // construction from fixed_string_base

   string1                             s8(  s3 );
   string1                             s9(  s3, 3 );
   string1                             s10( s3, 3, 4 );

   BOOST_TEST( s8  == "1234567890" );
   BOOST_TEST( s9  == "4567890" );
   BOOST_TEST( s10 == "4567" );

   // construction from ( first, last ) iterator pair

   const char *                        str = "ABCDEFGHI";

   string2                             s11( str, str + 5 );
   string2                             s12( s5.begin(), s5.end());

   BOOST_TEST( s11 == "ABCDE" );
   BOOST_TEST( s12 == s5 );
   BOOST_TEST( s12 == "abcdef" );

   return( 0 );
}
