// (C) Copyright 2003-2005: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// C++98[21.3.1] -- construct/copy/destroy

#include <boost/fixed_string/fixed_string.hpp>
#include <boost/test/minimal.hpp>

int test_main( int, char *[] )
{
   typedef boost::fixed_string< 15 > string1;
   typedef boost::fixed_string< 25 > string2;

   string1 s1;
   BOOST_CHECK( s1 == "" );
   BOOST_CHECK( s1.length() == 0 );

   // construction from primitive types

   string1 s2( "abcdef" );
   string2 s3( "abcdef", 3 );
   string2 s4( 5, '0' );

   BOOST_CHECK( s2 == "abcdef" );
   BOOST_CHECK( s3 == "abc" );
   BOOST_CHECK( s4 == "00000" );

   // construction from fixed_string

   string1 s5( s2 );
   string1 s6( s2, 3 );
   string1 s7( s2, 2, 3 );

   BOOST_CHECK( s2 == s5 );
   BOOST_CHECK( s5 == "abcdef" );
   BOOST_CHECK( s6 == "def" );
   BOOST_CHECK( s7 == "cde" );

   // assignment

   s1 = "1234567890";
   s2 = 'a';
   s3 = s1;

   BOOST_CHECK( s1 == "1234567890" );
   BOOST_CHECK( s2 == "a" );
   BOOST_CHECK( s3 == "1234567890" );

   // construction from fixed_string_base

   string1 s8(  s3 );
   string1 s9(  s3, 3 );
   string1 s10( s3, 3, 4 );

   BOOST_CHECK( s8  == "1234567890" );
   BOOST_CHECK( s9  == "4567890" );
   BOOST_CHECK( s10 == "4567" );

   // construction from ( first, last ) iterator pair

   const char * str = "ABCDEFGHI";

   string2 s11( str, str + 5 );
   string2 s12( s5.begin(), s5.end());

   BOOST_CHECK( s11 == "ABCDE" );
   BOOST_CHECK( s12 == s5 );
   BOOST_CHECK( s12 == "abcdef" );

   return 0;
}
